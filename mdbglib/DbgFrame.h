#pragma once

#include "stdafx.h"
#include "DbgArgument.h"

using namespace System;
using namespace System::Text;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgFrame
			{
			private:
				DebuggeeInfo^ m_info;
				ULONG64 m_instructionOffset;
				ULONG64 m_returnOffset;
				ULONG64 m_frameOffset;
				ULONG64 m_stackOffset;
				ULONG64 m_funcTableEntry;
				array<ULONG64>^ m_params;
				array<ULONG64>^ m_reserved;
				bool    m_virtual;
				ULONG   m_frameNumber;
				array<BYTE>^ m_scope;

				String ^m_functionName;
				Nullable<ULONG64> m_displacement;
				Nullable<ULONG> m_line;
				String ^m_fileName;
				array<DbgArgument^>^ m_arguments;

			internal:
				DbgFrame(DebuggeeInfo^ info, _DEBUG_STACK_FRAME frame, array<BYTE>^ scope);
				void ToLegacyStruct(_DEBUG_STACK_FRAME& frame);
			private:
				void GetFunctionNameAndDisplacement();
				void GetFileAndLine();
			public:
				property String ^FunctionName
				{
					String ^get();
				}
				property ULONG64 Displacement
				{
					ULONG64 get();
				}
				property String ^File
				{
					String ^get();
				}
				property UINT Line 
				{
					UINT get();
				}
				property array<DbgArgument^>^ Arguments
				{
					array<DbgArgument^>^ get();
				}
				virtual String^ ToString() override;
			};
		}
	}
}