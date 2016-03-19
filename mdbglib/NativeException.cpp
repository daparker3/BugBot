#include "stdafx.h"
#include "NativeException.h"
#include "Debuggee.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			NativeException::NativeException(Debuggee^ debuggee, PEXCEPTION_RECORD64 exr)
			{
				_exceptionCode = exr->ExceptionCode;
				_continuable = exr->ExceptionFlags != EXCEPTION_NONCONTINUABLE;
				if (exr->ExceptionRecord)
				{
					_innerException = gcnew NativeException(debuggee, (PEXCEPTION_RECORD64)exr->ExceptionRecord);
				}
				_exceptionAddress = exr->ExceptionAddress;
				_exceptionInformation = gcnew array<DWORD64>(exr->NumberParameters);
				for (DWORD i = 0; i < exr->NumberParameters; ++i)
				{
					_exceptionInformation[i] = exr->ExceptionInformation[i];
				}
				
				_debuggee = debuggee;
			}

			ExceptionFlags NativeException::GetExceptionFlags()
			{
				return (ExceptionFlags)_exceptionCode;
			}
		}
	}
}