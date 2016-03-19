#pragma once

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class Tools
			{
			public:
				static void ThrowDbgEngException(HRESULT hr);
				static void CheckHR(HRESULT hr);
				static LPCTSTR ConvertStringToLPCTSTR(String ^);
				static void FreeUnmanagedString(String ^managedString, LPCTSTR lstr);
			};
		}
	}
}