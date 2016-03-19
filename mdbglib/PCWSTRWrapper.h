#pragma once

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class PCWSTRWrapper// : public System::IDisposable
			{
			public:
				PCWSTRWrapper(String ^s);
				property IntPtr^ SzPtr
				{
					IntPtr^ get();
				}

				static PWSTR GetUnmanagedString(String ^s);
				static void FreeUnmanagedString(PWSTR s);
			private:
				bool disposed;
				IntPtr^ _szPtr;

				~PCWSTRWrapper(); //Dispose
				!PCWSTRWrapper(); //Finalize method
			};
		}
	}
}