#pragma once

#include "DebugOutputEventArgs.h"
#include "DebugEventArgs.h"

using namespace System;

#ifdef UNICODE
#undef CreateProcess
#endif

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			ref class Debuggee; //Just tell it that there is such a thing that Debuggee

			class OutputCallbackWrapper : public IDebugOutputCallbacksWide
			{
			private:
				LONG  m_refCount;
				gcroot<Debuggee^> *m_pdbg; 

			public:
				OutputCallbackWrapper(MS::Debuggers::DbgEng::Debuggee^ dbg);

				STDMETHOD_(ULONG, AddRef)(THIS);
				STDMETHOD_(ULONG, Release)(THIS);
				STDMETHOD(QueryInterface)(THIS_
					IN REFIID interfaceId,
					OUT PVOID* ppInterface);

				STDMETHOD(Output)(
					THIS_ 
					__in ULONG Mask,
					__in PCWSTR Text);

				~OutputCallbackWrapper() 
				{
					if(NULL != m_pdbg)
						delete m_pdbg; 
				}
			};

			class EventCallbackWrapper : public IDebugEventCallbacksWide
			{
			private:
				LONG m_refCount;
				gcroot<Debuggee^> *m_pdbg; 
				DWORD64 m_lastExAddr;

			public:
				EventCallbackWrapper(MS::Debuggers::DbgEng::Debuggee^ dbg);
				
				STDMETHOD_(ULONG, AddRef)(THIS);
				STDMETHOD_(ULONG, Release)(THIS);
				STDMETHOD(QueryInterface)(THIS_
					IN REFIID interfaceId,
					OUT PVOID* ppInterface);

				STDMETHOD(Breakpoint)(
					THIS_ 
					__in PDEBUG_BREAKPOINT2 Bp);
				STDMETHOD(ChangeDebuggeeState)(
					THIS_ 
					__in ULONG Flags,
					__in ULONG64 Argument);
				STDMETHOD(ChangeEngineState)(
					THIS_ 
					__in ULONG Flags,
					__in ULONG64 Argument);
				STDMETHOD(ChangeSymbolState)(
					THIS_ 
					__in ULONG Flags,
					__in ULONG64 Argument);
				STDMETHOD(CreateProcessW)(
					THIS_ 
					__in      ULONG64 ImageFileHandle,
					__in      ULONG64 Handle,
					__in      ULONG64 BaseOffset,
					__in      ULONG ModuleSize,
					__in_opt  PCWSTR ModuleName,
					__in_opt  PCWSTR ImageName,
					__in      ULONG CheckSum,
					__in      ULONG TimeDateStamp,
					__in      ULONG64 InitialThreadHandle,
					__in      ULONG64 ThreadDataOffset,
					__in      ULONG64 StartOffset
				);
				STDMETHOD(CreateThread)(
				  THIS_
				  __in  ULONG64 Handle,
				  __in  ULONG64 DataOffset,
				  __in  ULONG64 StartOffset
				);
				STDMETHOD(Exception)(
				  THIS_
				  __in  PEXCEPTION_RECORD64 Exception,
				  __in  ULONG FirstChance
				);
				STDMETHOD(ExitProcess)(
				  THIS_
				  __in  ULONG ExitCode
				);
				STDMETHOD(ExitThread)(
				  THIS_
				  __in  ULONG ExitCode
				);
				STDMETHOD(GetInterestMask)(
				  THIS_
				  __out PULONG Mask
				);
				STDMETHOD(LoadModule)(
				  __in            ULONG64 ImageFileHandle,
				  __in            ULONG64 BaseOffset,
				  __in            ULONG ModuleSize,
				  __in_opt        PCWSTR ModuleName,
				  __in_opt        PCWSTR ImageName,
				  __in            ULONG CheckSum,
				  __in            ULONG TimeDateStamp
				);
				STDMETHOD(SessionStatus)(
				  THIS_
				  __in  ULONG Status
				);
				STDMETHOD(SystemError)(
				  THIS_
				  __in  ULONG Error,
				  __in  ULONG Level
				);
				STDMETHOD(UnloadModule)(
				  THIS_
				  __in  PCWSTR ImageBaseName,
				  __in  ULONG64 BaseOffset
				);

				~EventCallbackWrapper() 
				{
					if(NULL != m_pdbg)
						delete m_pdbg; 
				}
			};
		}
	}
}

#ifdef UNICODE
#define CreateProcess CreateProcessW
#endif