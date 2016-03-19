#include "StdAfx.h"
#include "DebugSymbolGroup.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			String ^DebugSymbolGroup::GetSymbolName(IDebugSymbolGroup2* symbolGroup, ULONG index)
			{
				ULONG nameSize;
				Tools::CheckHR(symbolGroup->GetSymbolNameWide(index, NULL, 0, &nameSize));

				PWSTR pwName = NULL;
				try
				{
					pwName = new WCHAR[nameSize];
					Tools::CheckHR(symbolGroup->GetSymbolNameWide(index, pwName, nameSize, NULL));
					return gcnew String(pwName);
				}
				finally
				{
					delete[] pwName;
				}
			}
			ULONG64 DebugSymbolGroup::GetSymbolOffset(IDebugSymbolGroup2* symbolGroup, ULONG index)
			{
				ULONG64 result;
				Tools::CheckHR(symbolGroup->GetSymbolOffset(index, &result));
				return result;
			}

		}
	}
}
