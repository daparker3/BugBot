#pragma once
#include "Stdafx.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			ref class Debuggee;

			public ref class NativeException
			{
			private:
				Debuggee^ _debuggee;
				DWORD _exceptionCode;
				ULONG64 _exceptionAddress;
				array<DWORD64>^ _exceptionInformation;
				NativeException^ _innerException;
				bool _continuable;
			internal:
				NativeException(Debuggee^ debuggee, PEXCEPTION_RECORD64 exr);
			public:

				ExceptionFlags GetExceptionFlags();

				property DWORD ExceptionCode
				{
					DWORD get()
					{
						return _exceptionCode;
					}
				}

				property ULONG64 ExceptionAddress
				{
					ULONG64 get()
					{
						return _exceptionAddress;
					}
				}

				property array<DWORD64>^ ExceptionInformation
				{
					array<DWORD64>^ get()
					{
						return _exceptionInformation;
					}
				}

				property NativeException^ InnerException
				{
					NativeException^ get()
					{
						return _innerException;
					}
				}

				property bool Continuable
				{
					bool get()
					{
						return _continuable;
					}
				}
			};
		}
	}
}