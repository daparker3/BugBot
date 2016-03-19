#include "StdAfx.h"
#include "DbgFrame.h"

using namespace System;
using namespace System::Text;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgFrame::DbgFrame(DebuggeeInfo^ info, _DEBUG_STACK_FRAME frame, array<BYTE>^ scope)
			{
				this->m_info = info;
				this->m_scope = scope;

				this->m_instructionOffset = frame.InstructionOffset;
				this->m_returnOffset = frame.ReturnOffset;
				this->m_frameOffset = frame.FrameOffset;
				this->m_stackOffset = frame.StackOffset;
				this->m_funcTableEntry = frame.FuncTableEntry;
				this->m_virtual = frame.Virtual != 0;
				this->m_frameNumber = frame.FrameNumber;

				this->m_params = gcnew array<ULONG64>(4);
				for(int i=0;i<4;i++)
					this->m_params[i] = frame.Params[i];

				this->m_reserved = gcnew array<ULONG64>(6);
				for(int i=0;i<6;i++)
					this->m_reserved[i] = frame.Reserved[i];
			}
			void DbgFrame::ToLegacyStruct(_DEBUG_STACK_FRAME& frame)
			{
				frame.InstructionOffset = this->m_instructionOffset;
				frame.ReturnOffset = this->m_returnOffset;
				frame.FrameOffset = this->m_frameOffset;
				frame.StackOffset = this->m_stackOffset;
				frame.FuncTableEntry = this->m_funcTableEntry;
				frame.Virtual = this->m_virtual ? 1 : 0;
				frame.FrameNumber = this->m_frameNumber;

				for(int i=0;i<4;i++)
					frame.Params[i] = this->m_params[i];

				for(int i=0;i<6;i++)
					frame.Reserved[i] = this->m_reserved[i];
			}
			void DbgFrame::GetFunctionNameAndDisplacement()
			{
				try
				{
					ULONG64 displacement;
					this->m_info->DebugSymbols->GetNameByOffset(this->m_instructionOffset, this->m_functionName, displacement);
					this->m_displacement = Nullable<ULONG64>(displacement);
				}
				catch(System::Exception^)//(System::Runtime::InteropServices::COMException^)
				{
					this->m_functionName = "!Unknown";
					this->m_displacement = Nullable<ULONG64>(0);
				}

			}
			void DbgFrame::GetFileAndLine()
			{
				try
				{
					ULONG line;
					ULONG64 displacement;
					this->m_info->DebugSymbols->GetLineByOffset(this->m_instructionOffset, line, this->m_fileName, displacement);
					this->m_line = Nullable<ULONG>(line);
					//displacement is ignored here
				}
				catch(System::Runtime::InteropServices::COMException^)
				{
					this->m_fileName = "!Unknown";
					this->m_line = Nullable<ULONG>(0);
				}
			}

			String ^DbgFrame::FunctionName::get()
			{
				if(nullptr == this->m_functionName)
					this->GetFunctionNameAndDisplacement();

				return this->m_functionName;
			}

			ULONG64 DbgFrame::Displacement::get()
			{
				if(!this->m_displacement.HasValue)
					this->GetFunctionNameAndDisplacement();

				return this->m_displacement.Value;
			}

			String ^DbgFrame::File::get()
			{
				if(nullptr == this->m_fileName)
					this->GetFileAndLine();

				return this->m_fileName;
			}

			UINT DbgFrame::Line::get()
			{
				if(!this->m_line.HasValue)
					this->GetFileAndLine();

				return this->m_line.Value;
			}

			array<DbgArgument^>^ DbgFrame::Arguments::get()
			{
				if(nullptr != this->m_arguments)
					return this->m_arguments;

				IDebugSymbolGroup2* symbolGroup = NULL;
				BYTE* pScope = NULL;
				try
				{
					//Set scope to the current frame in call stack
					_DEBUG_STACK_FRAME me;
					this->ToLegacyStruct(me);

					ULONG scopeLength = 0;
					if(nullptr != this->m_scope)
					{
						scopeLength = this->m_scope->Length;
						pScope = new BYTE[scopeLength];
						for(ULONG i=0; i<scopeLength; i++)
						{
							pScope[i] = this->m_scope[i];
						}
					}

					Tools::CheckHR(this->m_info->Symbols->SetScope(0, &me, pScope, scopeLength));
					//Tools::CheckHR(this->m_info->Symbols->SetScope(0, &me, NULL, 0));
					//Get Function Arguments

					Tools::CheckHR(this->m_info->Symbols->GetScopeSymbolGroup2(DEBUG_SCOPE_GROUP_ARGUMENTS, NULL, &symbolGroup));

					ULONG numberSymbols;
					Tools::CheckHR(symbolGroup->GetNumberSymbols(&numberSymbols));

					array<DbgArgument^>^ result = gcnew array<DbgArgument^>(numberSymbols);
					for(ULONG i=0;i<numberSymbols;i++)
					{
						DEBUG_SYMBOL_ENTRY entry;
						Tools::CheckHR(symbolGroup->GetSymbolEntryInformation(i, &entry));
						DbgArgument^ a = gcnew DbgArgument(this->m_info, entry.Offset, entry.ModuleBase, entry.TypeId);
						a->Type->Size = entry.Size;
						a->Name = DebugSymbolGroup::GetSymbolName(symbolGroup, i);
						result[i] = a;
					}
					this->m_arguments = result;
				}
				finally
				{
               if (symbolGroup)
               {
                  symbolGroup->Release();
               }
					delete[] pScope;
				}
				return this->m_arguments;
			}
			String^ DbgFrame::ToString()
			{
				StringBuilder ^sb = gcnew StringBuilder(30);
				sb->Append(this->FunctionName);
				sb->Append("(");
				array<DbgArgument^>^ args = this->Arguments;
				for(int i=0;i<args->Length;i++)
				{
					sb->Append(args[i]->Name);
					if(i<args->Length-1)
						sb->Append(", ");
				}
				sb->Append(")");
				return sb->ToString();
			}
		}
	}
}
