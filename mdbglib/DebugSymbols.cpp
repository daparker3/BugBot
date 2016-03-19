#include "StdAfx.h"
#include "DebugSymbols.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebugSymbols::DebugSymbols(DbgSymbols* symbols)
			{
				m_symbols = symbols;
				m_symbols->AddRef();
				m_disposed = false;
			}

			DebugSymbols::~DebugSymbols()
			{
				this->!DebugSymbols();
				m_disposed = true;
			}

			DebugSymbols::!DebugSymbols()
			{
				if(NULL != m_symbols)
				{
					m_symbols->Release();
					m_symbols = NULL;
				}
			}

			String^ DebugSymbols::GetConstantName(ULONG64 module, ULONG typeId, ULONG64 value)
			{
//HRESULT
//  IDebugSymbols3::GetConstantName(
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN ULONG64  Value,
//    OUT OPTIONAL PWSTR  NameBuffer,
//    IN ULONG  NameBufferSize,
//    OUT OPTIONAL PULONG  NameSize
//    );
				PWSTR pwConstantName = NULL;
				try
				{
					ULONG nameSize;
					Tools::CheckHR(m_symbols->GetConstantNameWide(module, typeId, value, NULL, 0, &nameSize));
					pwConstantName = new WCHAR[nameSize];
					Tools::CheckHR(m_symbols->GetConstantNameWide(module, typeId, value, pwConstantName, nameSize, NULL));
					return gcnew String(pwConstantName);
				}
				finally
				{
					delete[] pwConstantName;
				}
			}
			array<Byte>^ DebugSymbols::ReadTypedDataPhysical(ULONG64 offset, ULONG64 module, ULONG typeId)
			{
//HRESULT
//  IDebugSymbols::ReadTypedDataPhysical(
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    OUT PVOID  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  BytesRead
//    );
				ULONG buffSize;
				Tools::CheckHR(m_symbols->ReadTypedDataPhysical(offset, module, typeId, NULL, 0, &buffSize));
				array<Byte>^ buff = gcnew array<Byte>(buffSize);
				pin_ptr<BYTE> buffPtr = &buff[0];
				Tools::CheckHR(m_symbols->ReadTypedDataPhysical(offset, module, typeId, buffPtr, buffSize, NULL));
				return buff;
			}
			void DebugSymbols::OutputTypedDataPhysical(DebugOutputControl outputControl, ULONG64 offset, ULONG64 module, ULONG typeId, TypeOptions flags)
			{
//HRESULT
//  IDebugSymbols::OutputTypedDataPhysical(
//    IN ULONG  OutputControl,
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN ULONG  Flags
//    );
				Tools::CheckHR(m_symbols->OutputTypedDataPhysical((ULONG) outputControl, offset, module, typeId, (ULONG)flags));
			}
			ULONG DebugSymbols::WriteTypedDataPhysical(ULONG64 offset, ULONG64 module, ULONG typeId, array<Byte>^ buff)
			{
//HRESULT
//  IDebugSymbols::WriteTypedDataPhysical(
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN PVOID  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  BytesWritten
//    );
				ULONG bytesWritten;
				pin_ptr<BYTE> buffPtr = &buff[0];
				Tools::CheckHR(m_symbols->WriteTypedDataPhysical(offset, module, typeId, buffPtr, buff->Length, &bytesWritten));
				return bytesWritten;
			}
			array<Byte>^ DebugSymbols::ReadTypedDataVirtual(ULONG64 offset, ULONG64 module, ULONG typeId)
			{
//HRESULT
//  IDebugSymbols::ReadTypedDataVirtual(
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    OUT PVOID  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  BytesRead
//    );
				ULONG buffSize;
				Tools::CheckHR(m_symbols->ReadTypedDataVirtual(offset, module, typeId, NULL, 0, &buffSize));
				array<Byte>^ buff = gcnew array<Byte>(buffSize);
				pin_ptr<BYTE> buffPtr = &buff[0];
				Tools::CheckHR(m_symbols->ReadTypedDataVirtual(offset, module, typeId, buffPtr, buffSize, NULL));
				return buff;
			}
			void DebugSymbols::OutputTypedDataVirtual(DebugOutputControl outputControl, ULONG64 offset, ULONG64 module, ULONG typeId, TypeOptions flags)
			{
//HRESULT
//  IDebugSymbols::OutputTypedDataVirtual(
//    IN ULONG  OutputControl,
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN ULONG  Flags
//    );
				Tools::CheckHR(m_symbols->OutputTypedDataVirtual((ULONG) outputControl, offset, module, typeId, (ULONG)flags));
			}
			ULONG DebugSymbols::WriteTypedDataVirtual(ULONG64 offset, ULONG64 module, ULONG typeId, array<Byte>^ buff)
			{
//HRESULT
//  IDebugSymbols::WriteTypedDataVirtual(
//    IN ULONG64  Offset,
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN PVOID  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  BytesWritten
//    );
				ULONG bytesWritten;
				pin_ptr<BYTE> buffPtr = &buff[0];
				Tools::CheckHR(m_symbols->WriteTypedDataVirtual(offset, module, typeId, buffPtr, buff->Length, &bytesWritten));
				return bytesWritten;
			}
			String^ DebugSymbols::GetFieldName(ULONG64 module, ULONG typeId, ULONG fieldIndex)
			{
//HRESULT
//  IDebugSymbols3::GetFieldNameWide(
//    IN ULONG64  Module,
//    IN ULONG  TypeId,
//    IN ULONG  FieldIndex,
//    OUT OPTIONAL PSTR  NameBuffer,
//    IN ULONG  NameBufferSize,
//    OUT OPTIONAL PULONG  NameSize
//    );
				PWSTR pFieldName = NULL;
				try
				{
					ULONG nameSize;
					Tools::CheckHR(m_symbols->GetFieldNameWide(module, typeId, fieldIndex, NULL, 0, &nameSize));
					pFieldName = new WCHAR[nameSize];
					Tools::CheckHR(m_symbols->GetFieldNameWide(module, typeId, fieldIndex, pFieldName, nameSize, NULL));
					String^ fieldName = gcnew String(pFieldName);
					return fieldName;
				}
				finally
				{
					delete[] pFieldName;
				}
			}

			String^ DebugSymbols::GetImagePath()
			{
//IDebugSymbols3::GetImagePathWide(
//    OUT OPTIONAL PWSTR  Buffer,
//    IN ULONG  BufferSize,
//    Out OPTIONAL PULONG  PathSize
//    );
				PWSTR pImagePath = NULL;
				try
				{
					ULONG pathSize;
					Tools::CheckHR(m_symbols->GetImagePath(NULL, 0, &pathSize));
					pImagePath = new WCHAR[pathSize];
					Tools::CheckHR(m_symbols->GetImagePathWide(pImagePath, pathSize, NULL));
					String^ imagePath = gcnew String(pImagePath);
					return imagePath;
				}
				finally
				{
					delete[] pImagePath;
				}
			}
			void DebugSymbols::SetImagePath(String^ value)
			{
//IdebugSymbols3::SetImagePathWide(
//    IN PCWSTR  Path
//    );
				pin_ptr<const wchar_t> pImagePath = PtrToStringChars(value);
				Tools::CheckHR(m_symbols->SetImagePathWide(pImagePath));
			}
			void DebugSymbols::GetLineByOffset(ULONG64 offset, [Out] ULONG %line, [Out] String ^%fileName, [Out] ULONG64 %displacement)
			{
//IdebugSymbols3::GetLineByOffsetWide(
//    IN ULONG64  Offset,
//    OUT OPTIONAL PULONG  Line,
//    OUT OPTIONAL PWSTR  FileBuffer,
//    IN ULONG  FileBufferSize,
//    OUT OPTIONAL PULONG  FileSize,
//    OUT OPTIONAL PULONG64  Displacement
//    );
				PWSTR pwFile = NULL;
				try
				{
					ULONG fileNameSize;
					Tools::CheckHR(m_symbols->GetLineByOffsetWide(offset, NULL, NULL, 0, &fileNameSize, NULL));
					pwFile = new WCHAR[fileNameSize];
					ULONG mLine;
					Tools::CheckHR(m_symbols->GetLineByOffsetWide(offset, &mLine, pwFile, fileNameSize, &fileNameSize, NULL));
					line = mLine;
					fileName = gcnew String(pwFile);
				}
				finally
				{
					delete[] pwFile;
				}
			}



			void DebugSymbols::GetFieldTypeAndOffset(ULONG64 moduleBase, ULONG typeId, String ^fieldName, [Out] ULONG %fieldTypeId, [Out] ULONG %fieldOffset)
			{
//IDebugSymbols3::GetFieldTypeAndOffsetWide(
//    IN ULONG64  Module,
//    IN ULONG  ContainerTypeId,
//    IN PCWSTR  Field,
//    OUT OPTIONAL PULONG  FieldTypeId,
//    OUT OPTIONAL PULONG  Offset
//    );
				ULONG fType;
				ULONG fOffset;
				pin_ptr<const wchar_t> pFieldName = PtrToStringChars(fieldName);
				Tools::CheckHR(m_symbols->GetFieldTypeAndOffsetWide(moduleBase, typeId, pFieldName, &fType, &fOffset));
				fieldTypeId = fType;
				fieldOffset = fOffset;
			}
			void DebugSymbols::GetNameByOffset(ULONG64 offset, [Out] String ^%functionName, [Out] ULONG64 %displacement)
			{
				PWSTR pwFunctionName = NULL;
				try
				{
					ULONG nameSize;
					HRESULT hr = m_symbols->GetNameByOffsetWide(offset,NULL, 0, &nameSize, NULL);
					if (hr == -2147467259) //Number taken from mdbgeng.dll
					{
						throw gcnew ArgumentException("No symbol found at specified location.", "offset");
					}
					Tools::CheckHR(hr);

					pwFunctionName = new WCHAR[nameSize];

					ULONG64 mDisplacement;
					Tools::CheckHR(m_symbols->GetNameByOffsetWide(offset, pwFunctionName, nameSize, NULL, &mDisplacement));
					displacement = mDisplacement;

					functionName = gcnew String(pwFunctionName);
				}
				finally
				{
					delete[] pwFunctionName;
				}

			}

#pragma region Scope

			void DebugSymbols::SetScopeFromStoredEvent()
			{
				Tools::CheckHR(m_symbols->SetScopeFromStoredEvent());
			}
			ULONG DebugSymbols::GetCurrentScopeFrameIndex()
			{
				ULONG result;
				Tools::CheckHR(m_symbols->GetCurrentScopeFrameIndex(&result));
				return result;
			}
			void DebugSymbols::SetScopeFrameByIndex(ULONG index)
			{
				Tools::CheckHR(m_symbols->SetScopeFrameByIndex(index));
			}
			void DebugSymbols::SetScope(ULONG64 instructionOffset, PDEBUG_STACK_FRAME pScopeFrame, PVOID pScopeContext, ULONG scopeContextSize)
			{
				Tools::CheckHR(m_symbols->SetScope(instructionOffset, pScopeFrame, pScopeContext, scopeContextSize));
			}
			void DebugSymbols::GetScope(PULONG64 pInstructionOffset, PDEBUG_STACK_FRAME pScopeFrame, PVOID pScopeContext, ULONG scopeContextSize)
			{
				Tools::CheckHR(m_symbols->GetScope(pInstructionOffset, pScopeFrame, pScopeContext, scopeContextSize));
			}

#pragma endregion

#pragma region Symbol Location
			void DebugSymbols::SetSymbolPath(String ^symbolPath)
			{
//DbgSymbols::SetSymbolPathWide(
//    IN PWSTR  Path
//    );
				pin_ptr<const wchar_t> pSymbolPath = PtrToStringChars(symbolPath);
				Tools::CheckHR(m_symbols->SetSymbolPathWide(pSymbolPath));
			}
			String ^DebugSymbols::GetSymbolPath()
			{
//DbgSymbols::GetSymbolPathWide(
//    OUT OPTIONAL PSTR  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  PathSize
//    );
				PWSTR pwSymbolPath = NULL;
				try
				{
					ULONG pathSize;
					Tools::CheckHR(m_symbols->GetSymbolPathWide(NULL, 0, &pathSize));
					pwSymbolPath = new WCHAR[pathSize];
					Tools::CheckHR(m_symbols->GetSymbolPathWide(pwSymbolPath, pathSize, NULL));
					return gcnew String(pwSymbolPath);
				}
				finally
				{
					delete[] pwSymbolPath;
				}
			}
			void DebugSymbols::AppendSymbolPath(String ^symbolPath)
			{
//DbgSymbols::AppendSymbolPathWide(
//    IN PCWSTR  Addition
//    );
				pin_ptr<const wchar_t> pSymbolPath = PtrToStringChars(symbolPath);
				Tools::CheckHR(m_symbols->AppendSymbolPathWide(pSymbolPath));
			}
#pragma endregion

#pragma region Module
			String^ DebugSymbols::GetModuleNameString(ModuleNameType nameType, ULONG index, ULONG64 base, ULONG nameSize)
			{
				PWSTR pwName = NULL;
				try
				{
					pwName = new WCHAR[nameSize];
					Tools::CheckHR(m_symbols->GetModuleNameStringWide((ULONG)nameType, index, base, pwName, nameSize, NULL));
					return gcnew String(pwName);
				}
				finally
				{
					delete[] pwName;
				}
			}
			void DebugSymbols::GetModuleByModuleName(String^ name, ULONG startIndex, GetMethodFlags flags, [Out] ULONG %index, [Out] ULONG64 %base)
			{
//HRESULT
//  IDebugSymbols3::GetModuleByModuleName2Wide(
//    IN PCWSTR  Name,
//    IN ULONG  StartIndex,
//    IN ULONG  Flags,
//    OUT OPTIONAL PULONG  Index,
//    OUT OPTIONAL PULONG64  Base
//    );
				pin_ptr<const wchar_t> pName = PtrToStringChars(name);
				ULONG mIndex;
				ULONG64 mBase;
				Tools::CheckHR(m_symbols->GetModuleByModuleName2Wide(pName, startIndex, (ULONG)flags, &mIndex, &mBase));
				index = mIndex;
				base = mBase;
			}
			void DebugSymbols::GetModuleByModuleName(String^ name, [Out] ULONG %index, [Out] ULONG64 %base)
			{
				return this->GetModuleByModuleName(name, 0, GetMethodFlags::Default, index, base);
			}

#pragma endregion

#pragma region Type
			ULONG DebugSymbols::GetTypeId(ULONG64 moduleBase, String ^typeName)
			{
				ULONG typeId;
				pin_ptr<const wchar_t> pResultObjectTypeName = PtrToStringChars(typeName);
				Tools::CheckHR(m_symbols->GetTypeIdWide(moduleBase, pResultObjectTypeName, &typeId));
				return typeId;
			}
#pragma endregion

			void DebugSymbols::GetSymbolType(String ^symbol, [Out] ULONG %typeId, [Out] ULONG64 %moduleBase)
			{
//HRESULT
//  DbgSymbols::GetSymbolTypeIdWide(
//    IN PCWSTR  Symbol,
//    OUT PULONG  TypeId,
//    OUT OPTIONAL PULONG64  Module
//    );
				ULONG mTypeId;
				ULONG64 mModuleBase;
				pin_ptr<const wchar_t> pSymbol = PtrToStringChars(symbol);
				Tools::CheckHR(m_symbols->GetSymbolTypeIdWide(pSymbol, &mTypeId, &mModuleBase));
				typeId = mTypeId;
				moduleBase = mModuleBase;
			}
		}
	}
}
//throw gcnew NotImplementedException("Please implement me");