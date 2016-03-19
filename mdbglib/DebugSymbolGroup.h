#pragma once

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			private ref class DebugSymbolGroup
			{
			internal:
				static String ^GetSymbolName(IDebugSymbolGroup2* symbolGroup, ULONG index);
				static ULONG64 GetSymbolOffset(IDebugSymbolGroup2* symbolGroup, ULONG index);
			};
		}
	}
}