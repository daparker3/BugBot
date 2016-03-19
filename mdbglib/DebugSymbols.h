#pragma once

#include "DbgModule.h"
#include "DbgType.h"

using namespace System;
using namespace Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugSymbols
			{
			private:
				bool m_disposed;
				DbgSymbols *m_symbols;

			internal:
				DebugSymbols(DbgSymbols *);
				~DebugSymbols();
				!DebugSymbols();

			public:
				String^ GetConstantName(ULONG64 module, ULONG typeId, ULONG64 value);
				array<Byte>^ ReadTypedDataPhysical(ULONG64 offset, ULONG64 module, ULONG typeId);
				void OutputTypedDataPhysical(DebugOutputControl outputControl, ULONG64 offset, ULONG64 module, ULONG typeId, TypeOptions flags);
				ULONG WriteTypedDataPhysical(ULONG64 offset, ULONG64 module, ULONG typeId, array<byte>^ buff);
				array<Byte>^ ReadTypedDataVirtual(ULONG64 offset, ULONG64 module, ULONG typeId);
				void OutputTypedDataVirtual(DebugOutputControl outputControl, ULONG64 offset, ULONG64 module, ULONG typeId, TypeOptions flags);
				ULONG WriteTypedDataVirtual(ULONG64 offset, ULONG64 module, ULONG typeId, array<byte>^ buff);
				String^ GetFieldName(ULONG64 module, ULONG typeId, ULONG fieldIndex);

				String^ GetImagePath();
				void SetImagePath(String^ value);
				void GetLineByOffset(ULONG64 offset, [Out] ULONG %line, [Out] String ^%fileName, [Out] ULONG64 %displacement);


				void GetFieldTypeAndOffset(ULONG64 moduleBase, ULONG typeId, String ^fieldName, [Out] ULONG %fieldTypeId, [Out] ULONG %fieldOffset);
				void GetNameByOffset(ULONG64 offset, [Out] String ^%functionName, [Out] ULONG64 %displacement);

#pragma region Scope
				void SetScopeFromStoredEvent();
				ULONG GetCurrentScopeFrameIndex();
				void SetScopeFrameByIndex(ULONG index);
				void SetScope(ULONG64 instructionOffset, PDEBUG_STACK_FRAME pScopeFrame, PVOID pScopeContext, ULONG scopeContextSize);
				void GetScope(PULONG64 pInstructionOffset, PDEBUG_STACK_FRAME pScopeFrame, PVOID pScopeContext, ULONG scopeContextSize);
#pragma endregion

#pragma region Symbol Location
				void SetSymbolPath(String ^symbolPath);
				String^ GetSymbolPath();
				void AppendSymbolPath(String ^symbolPath);
#pragma endregion

#pragma region Module
				String^ GetModuleNameString(ModuleNameType nameType, ULONG index, ULONG64 base, ULONG nameSize);
				void GetModuleByModuleName(String^ name, ULONG startIndex, GetMethodFlags flags, [Out] ULONG %index, [Out] ULONG64 %base);
				void GetModuleByModuleName(String^ name, [Out] ULONG %index, [Out] ULONG64 %base);
#pragma endregion

#pragma region Type
				ULONG GetTypeId(ULONG64 moduleBase, String ^typeName);
#pragma endregion

				void GetSymbolType(String^ symbol, [Out] ULONG %typeId, [Out] ULONG64 %moduleBase);
			};
		}
	}
}
