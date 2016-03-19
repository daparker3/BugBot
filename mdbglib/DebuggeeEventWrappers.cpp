#include "StdAfx.h"
#include "DebuggeeEventWrappers.h"
#include "Debuggee.h"

#define DEBUG_INTEREST_MASK \
(DEBUG_EVENT_BREAKPOINT | DEBUG_EVENT_EXCEPTION | DEBUG_EVENT_CREATE_THREAD \
| DEBUG_EVENT_EXIT_THREAD | DEBUG_EVENT_CREATE_PROCESS | DEBUG_EVENT_EXIT_PROCESS \
| DEBUG_EVENT_LOAD_MODULE | DEBUG_EVENT_UNLOAD_MODULE | DEBUG_EVENT_SYSTEM_ERROR \
| DEBUG_EVENT_SESSION_STATUS)

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			OutputCallbackWrapper::OutputCallbackWrapper(Debuggee ^dbg) : m_refCount(0), m_pdbg(new gcroot<Debuggee^>(dbg))
			{
			} 

			STDMETHODIMP OutputCallbackWrapper::Output(__in ULONG mask, __in PCWSTR text)
			{
				if(NULL == this->m_pdbg) //Very unlikely considering the only constructor
					return S_FALSE;

				DbgEng::OutputFlags mmask = static_cast<DbgEng::OutputFlags>(mask);
				String ^mtext = gcnew String(text);
				(*(this->m_pdbg))->OnDebugOutput(mmask, mtext);
				return S_OK;
			}

			STDMETHODIMP_(ULONG) OutputCallbackWrapper::AddRef(THIS) 
			{
				InterlockedIncrement(&m_refCount);
				return m_refCount;
			}

			STDMETHODIMP_(ULONG) OutputCallbackWrapper::Release(THIS) 
			{
				LONG retVal;
				InterlockedDecrement(&m_refCount);
				retVal = m_refCount;
				if (retVal == 0) 
				{
					delete this;
				}
				return retVal;
			}

			STDMETHODIMP OutputCallbackWrapper::QueryInterface(THIS_
				IN REFIID interfaceId,
				OUT PVOID* ppInterface) 
			{
				*ppInterface = 0;
				HRESULT res = E_NOINTERFACE;
				if (TRUE == IsEqualIID(interfaceId, __uuidof(IUnknown)) || TRUE == IsEqualIID(interfaceId, __uuidof(IDebugOutputCallbacks))) 
				{
					*ppInterface = (IDebugOutputCallbacks*) this;
					AddRef();
					res = S_OK;
				}
				return res;
			}
			
			EventCallbackWrapper::EventCallbackWrapper(MS::Debuggers::DbgEng::Debuggee^ dbg)
				: m_refCount(0), m_pdbg(new gcroot<Debuggee^>(dbg))
			{
				m_lastExAddr = 0;
			}

			STDMETHODIMP_(ULONG) EventCallbackWrapper::AddRef(THIS) 
			{
				InterlockedIncrement(&m_refCount);
				return m_refCount;
			}

			STDMETHODIMP_(ULONG) EventCallbackWrapper::Release(THIS) 
			{
				LONG retVal;
				InterlockedDecrement(&m_refCount);
				retVal = m_refCount;
				if (retVal == 0) 
				{
					delete this;
				}
				return retVal;
			}

			STDMETHODIMP EventCallbackWrapper::QueryInterface(THIS_
				IN REFIID interfaceId,
				OUT PVOID* ppInterface) 
			{
				*ppInterface = 0;
				HRESULT res = E_NOINTERFACE;
				if (TRUE == IsEqualIID(interfaceId, __uuidof(IUnknown)) || TRUE == IsEqualIID(interfaceId, __uuidof(IDebugEventCallbacksWide))) 
				{
					*ppInterface = (IDebugEventCallbacksWide*) this;
					AddRef();
					res = S_OK;
				}
				return res;
			}

			STDMETHODIMP EventCallbackWrapper::Breakpoint(
				THIS_ 
				__in PDEBUG_BREAKPOINT2 Bp)
			{
				DbgBreak^ breakpoint = gcnew DbgBreak(Bp);
				return (HRESULT)(*(this->m_pdbg))->OnBreakpoint(breakpoint);
			}

			STDMETHODIMP EventCallbackWrapper::ChangeDebuggeeState(
				THIS_ 
				__in ULONG Flags,
				__in ULONG64 Argument)
			{
				return S_OK;
			}

			STDMETHODIMP EventCallbackWrapper::ChangeEngineState(
				THIS_ 
				__in ULONG Flags,
				__in ULONG64 Argument)
			{
				return S_OK;
			}

			STDMETHODIMP EventCallbackWrapper::ChangeSymbolState(
				THIS_ 
				__in ULONG Flags,
				__in ULONG64 Argument)
			{
				return S_OK;
			}

			STDMETHODIMP EventCallbackWrapper::CreateProcessW(
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
			)
			{	
				return (HRESULT)(*(this->m_pdbg))->OnProcessCreated(
					ImageFileHandle,
					Handle,
					BaseOffset,
					ModuleSize,
					gcnew String(ModuleName),
					gcnew String(ImageName),
					CheckSum,
					TimeDateStamp,
					InitialThreadHandle,
					ThreadDataOffset,
					StartOffset
				);
			}

			STDMETHODIMP EventCallbackWrapper::CreateThread(
				THIS_
				__in  ULONG64 Handle,
				__in  ULONG64 DataOffset,
				__in  ULONG64 StartOffset
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnThreadCreated(Handle, DataOffset, StartOffset);
			}

			STDMETHODIMP EventCallbackWrapper::Exception(
				THIS_
				__in  PEXCEPTION_RECORD64 Exception,
				__in  ULONG FirstChance
			)
			{
				if (Exception->ExceptionAddress == m_lastExAddr)
				{
					// We already got this exception. In order not to break our OM, just toss it out here before reporting it to the user.
					// The debugged process should exit.
					return DEBUG_STATUS_GO_NOT_HANDLED;
				}
				else
				{
					NativeException^ ex = gcnew NativeException(*(this->m_pdbg), Exception);
					if (!FirstChance)
					{
						m_lastExAddr = Exception->ExceptionAddress;
					}
					return (HRESULT)(*(this->m_pdbg))->OnException(ex, FirstChance != 0); 
				}
			}

			STDMETHODIMP EventCallbackWrapper::ExitProcess(
				THIS_
				__in  ULONG ExitCode
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnProcessExited(ExitCode);
			}

			STDMETHODIMP EventCallbackWrapper::ExitThread(
				THIS_
				__in  ULONG ExitCode
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnThreadExited(ExitCode);
			}

			STDMETHODIMP EventCallbackWrapper::LoadModule(
				THIS_ 
				__in            ULONG64 ImageFileHandle,
				__in            ULONG64 BaseOffset,
				__in            ULONG ModuleSize,
				__in_opt        PCWSTR ModuleName,
				__in_opt        PCWSTR ImageName,
				__in            ULONG CheckSum,
				__in            ULONG TimeDateStamp
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnModuleLoaded(ImageFileHandle, BaseOffset, ModuleSize, gcnew String(ModuleName), gcnew String(ImageName), CheckSum, TimeDateStamp);
			}

			STDMETHODIMP EventCallbackWrapper::SessionStatus(
				THIS_ 
				__in  ULONG Status
			)
			{
				(*(this->m_pdbg))->OnSessionStatus((SessionStatusFlags)Status);
				return S_OK;
			}

			STDMETHODIMP EventCallbackWrapper::SystemError(
				THIS_ 
				__in  ULONG Error,
				__in  ULONG Level
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnSystemError(Error, Level);
			}

			STDMETHODIMP EventCallbackWrapper::UnloadModule(
				THIS_ 
				__in  PCWSTR ImageBaseName,
				__in  ULONG64 BaseOffset
			)
			{
				return (HRESULT)(*(this->m_pdbg))->OnModuleUnloaded(gcnew String(ImageBaseName), BaseOffset);;
			}

			STDMETHODIMP EventCallbackWrapper::GetInterestMask(
				THIS_ 
				__out PULONG Mask
			)
			{
				*Mask = DEBUG_INTEREST_MASK;
				return S_OK;
			}
		}
	}
}