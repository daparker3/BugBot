#include "StdAfx.h"
#include "Debuggee.h"
#include "Tools.h"
#include "DebugOutputEventArgs.h"
#include "DebuggeeEventWrappers.h"

using namespace System;
using namespace Runtime::InteropServices;

#define MAX_PROCESSES 255

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
#pragma region Constructors/Destructor/Finalizer
			Debuggee::Debuggee()
			{
				/*
				HRESULT
				  DebugCreate(
					IN REFIID  InterfaceId,
					OUT PVOID *  Interface
					);
				*/
				DbgClient* debugClient = NULL;
				Tools::CheckHR(DebugCreate(__uuidof(DbgClient), (void **)&debugClient));
				this->m_info = gcnew DebuggeeInfo(debugClient);
				this->m_procServer = 0;
				this->disposed = false;

				this->m_nativeEventCall = new EventCallbackWrapper(this);
				this->m_nativeEventCall->AddRef();
				Tools::CheckHR(this->m_info->Client->SetEventCallbacksWide((IDebugEventCallbacksWide*)this->m_nativeEventCall));

				this->m_nativeOutCall = new OutputCallbackWrapper(this);
				this->m_nativeOutCall->AddRef();
				Tools::CheckHR(this->m_info->Client->SetOutputCallbacksWide((IDebugOutputCallbacksWide*)this->m_nativeOutCall));

				m_lastArgs = gcnew List<DebuggerEventArgs^>();
			}
			Debuggee::~Debuggee() //Dispose
			{
				if (!this->disposed)
				{
					//Clean managed resources here
					if (NULL != m_info->Client)
					{
						if (this->m_procServer)
						{
							Tools::CheckHR(m_info->Client->DisconnectProcessServer(this->m_procServer));
							this->m_procServer = 0;
						}
						else
						{
							HRESULT hr = this->m_info->Client->EndSession(DEBUG_END_ACTIVE_DETACH);
							if (!SUCCEEDED(hr))
							{
								this->m_info->Client->EndSession(DEBUG_END_PASSIVE);
							}
						}
					}
					this->m_info->~DebuggeeInfo();
					this->disposed = true;
					this->!Debuggee();
				}
				//No need to call SuppressFinalize, the compiler does it for you
			}
			Debuggee::!Debuggee() //Finalize method
			{
				//Clean unmanaged resources here
				if (NULL != this->m_nativeOutCall)
					this->m_nativeOutCall->Release();
				if (NULL != this->m_nativeEventCall)
					this->m_nativeEventCall->Release();
			}

			Debuggee^ Debuggee::OpenDumpFile(System::String ^dumpFilePath, System::String ^symbolPath)
			{
				Debuggee^ result = gcnew Debuggee();
				result->SymbolPath::set(symbolPath);
				result->OpenDumpFile(dumpFilePath);
				result->WaitForEvent(0);
				return result;
			}

			Debuggee^ Debuggee::ConnectToProcessServer(String ^serverOptions)
			{
				Debuggee^ result = gcnew Debuggee();
				pin_ptr<const wchar_t> pServerOptions = PtrToStringChars(serverOptions);
				pin_ptr<ULONG64> pProcServer = &result->m_procServer;
				Tools::CheckHR(result->m_info->Client->ConnectProcessServerWide(pServerOptions, pProcServer));
				return result;
			}

			Debuggee^ Debuggee::ConnectToKernel(String^ serverOptions)
			{
				Debuggee^ result = gcnew Debuggee();
				pin_ptr<const wchar_t> pServerOptions = nullptr;
				if (!String::IsNullOrEmpty(serverOptions))
					pServerOptions = PtrToStringChars(serverOptions);
				Tools::CheckHR(result->m_info->Client->AttachKernelWide(DEBUG_ATTACH_KERNEL_CONNECTION, pServerOptions));
				return result;
			}

#pragma endregion

#pragma region Process enumeration

			String^ Debuggee::GetProcessName(ULONG processId, GetRunningProcessDescriptionFlags flags)
			{
				WCHAR procName[MAX_PATH] = { 0 };
				ULONG szProcName;
				flags = flags | GetRunningProcessDescriptionFlags::NoMtsPackages | GetRunningProcessDescriptionFlags::NoCommandLine;
				HRESULT hr = m_info->Client->GetRunningProcessDescriptionWide(m_procServer, processId, (ULONG)flags, procName, MAX_PATH, &szProcName, NULL, 0, NULL);
				if (!SUCCEEDED(hr))
				{
					wcscat_s(procName, L"");
				}
				return gcnew String(procName);
			}

			ULONG Debuggee::GetSystemId(String^ processName, GetRunningProcessSystemIdByExecutableNameFlags flags)
			{
				pin_ptr<const wchar_t> pProcessName = PtrToStringChars(processName);
				ULONG ret = 0;
				Tools::CheckHR(m_info->Client->GetRunningProcessSystemIdByExecutableNameWide(m_procServer, pProcessName, (ULONG)flags, &ret));
				return ret;
			}

			array<ULONG>^ Debuggee::Processes::get()
			{
				ULONG processes[MAX_PROCESSES];
				ULONG ctProcesses = 0;
				Tools::CheckHR(m_info->Client->GetRunningProcessSystemIds(m_procServer, processes, MAX_PROCESSES, &ctProcesses));
				array<ULONG>^ ret = gcnew array<ULONG>(ctProcesses);

				for (ULONG i = 0; i < ctProcesses; ++i)
				{
					ret[i] = processes[i];
				}

				return ret;
			}

#pragma region Symbols Location
			void Debuggee::SymbolPath::set(String ^symbolPath)
			{
				this->m_info->DebugSymbols->SetSymbolPath(symbolPath);
			}
			String ^Debuggee::SymbolPath::get()
			{
				return this->m_info->DebugSymbols->GetSymbolPath();
			}
			void Debuggee::SymbolPathAppend(String ^symbolPath)
			{
				this->m_info->DebugSymbols->AppendSymbolPath(symbolPath);
			}
#pragma endregion

#pragma region Dump Files

			void Debuggee::OpenDumpFile(String ^dumpFilePath)
			{
				this->m_info->DebugClient->OpenDumpFile(dumpFilePath, 0);
			}

			void Debuggee::OpenDumpFile(IntPtr handle)
			{
				this->m_info->DebugClient->OpenDumpFile(nullptr, handle.ToInt64());
			}

			void Debuggee::AddDumpFile(String ^dumpFilePath)
			{
				this->m_info->DebugClient->AddDumpFileInformation(dumpFilePath, 0);
			}

			void Debuggee::AddDumpFile(IntPtr handle)
			{
				this->m_info->DebugClient->AddDumpFileInformation(nullptr, handle.ToInt64());
			}

			void Debuggee::WriteDumpFile(String ^path, DumpType type, DumpFlags flags, String ^comment)
			{
				this->m_info->DebugClient->WriteDumpFile(path, 0, (ULONG)type, (ULONG)flags, comment);
			}

			String ^Debuggee::GetDumpFile(ULONG index)
			{
				return this->m_info->DebugClient->GetDumpFile(index);
			}
#pragma endregion

#pragma region Attach
			void Debuggee::CreateProcess(String ^commandLine, String ^initialDirectory)
			{
				this->m_info->DebugClient->CreateProcess(commandLine, initialDirectory);
			}
			void Debuggee::CreateAndAttachProcess(String ^commandLine, String ^initialDirectory)
			{
				this->m_info->DebugClient->CreateProcessAndAttach(commandLine, initialDirectory);
			}
			void Debuggee::AttachProcess(ULONG processId, AttachFlags flags)
			{
				//TODO:Check flags here
				this->m_info->DebugClient->AttachProcess(m_procServer, processId, (ULONG)flags);
			}
			void Debuggee::DetachCurrentProcess()
			{
				this->m_info->DebugClient->DetachCurrentProcess();
			}
			void Debuggee::DetachProcesses()
			{
				this->m_info->DebugClient->DetachProcesses();
			}
#pragma endregion

#pragma region Threads
			array<DbgThread^>^ Debuggee::Threads::get()
			{
				array<DbgThread^>^ result = nullptr;

				ULONG ulNumberThreads;
				PULONG pulIds = NULL;
				PULONG pulSysIds = NULL;
				try
				{
					Tools::CheckHR(this->m_info->SystemObjects->GetNumberThreads(&ulNumberThreads));
					pulIds = new ULONG[ulNumberThreads];
					pulSysIds = new ULONG[ulNumberThreads];
					Tools::CheckHR(this->m_info->SystemObjects->GetThreadIdsByIndex(0, ulNumberThreads, pulIds, pulSysIds));

					result = gcnew array<DbgThread^>(ulNumberThreads);

					for (ULONG i = 0; i < ulNumberThreads; i++)
					{
						result[i] = gcnew DbgThread(this->m_info, pulIds[i], pulSysIds[i]);
					}
				}
				finally
				{
					if (pulIds)
					{
						delete[] pulIds;
					}
					if (pulSysIds)
					{
						delete[] pulSysIds;
					}
				}

				return result;
			}
			ULONG Debuggee::EventThreadId::get()
			{
				ULONG eventThread = this->m_info->DebugSystemObjects->GetEventThreadId();
				return eventThread;
			}
			DbgThread^ Debuggee::EventThread::get()
			{
				ULONG eventThread = this->m_info->DebugSystemObjects->GetEventThreadId();
				ULONG threadSystemId = this->m_info->DebugSystemObjects->GetCurrentThreadSystemId();
				return gcnew DbgThread(m_info, eventThread, threadSystemId);
			}
			ULONG Debuggee::EventProcessId::get()
			{
				ULONG eventProcess = this->m_info->DebugSystemObjects->GetEventProcessId();
				return eventProcess;
			}
			DbgProcess^ Debuggee::EventProcess::get()
			{
				ULONG eventProcess = this->m_info->DebugSystemObjects->GetEventProcessId();
				DbgProcess^ result = gcnew DbgProcess(m_info, eventProcess);
				return result;
			}
			DbgProcess^ Debuggee::CurrentProcess::get()
			{
				ULONG currentProcessID = this->m_info->DebugSystemObjects->GetCurrentProcessId();
				DbgProcess^ result = gcnew DbgProcess(m_info, currentProcessID);
				return result;
			}
			ULONG Debuggee::CurrentProcessId::get()
			{
				return this->m_info->DebugSystemObjects->GetCurrentProcessId();
			}
			void Debuggee::CurrentProcessId::set(ULONG processId)
			{
				this->m_info->DebugSystemObjects->SetCurrentProcessId(processId);
			}
			DbgThread^ Debuggee::CurrentThread::get()
			{
				ULONG threadId = this->m_info->DebugSystemObjects->GetCurrentThreadId();
				ULONG threadSystemId = this->m_info->DebugSystemObjects->GetCurrentThreadSystemId();
				DbgThread^ result = gcnew DbgThread(this->m_info, threadId, threadSystemId);
				return result;
			}
			ULONG Debuggee::CurrentThreadId::get()
			{
				return this->m_info->DebugSystemObjects->GetCurrentThreadId();
			}
			void Debuggee::CurrentThreadId::set(ULONG threadId)
			{
				this->m_info->DebugSystemObjects->SetCurrentThreadId(threadId);
			}
			array<DbgFrame^>^ Debuggee::GetStoredStackTrace()
			{
				this->m_info->DebugSymbols->SetScopeFromStoredEvent();
				return this->m_info->DebugControl->GetStoredStackTrace(1000);
			}
#pragma endregion
#pragma region Events
			DebuggerStatus Debuggee::Update()
			{
				HRESULT hr;

				hr = this->m_info->Control->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);
				if (hr != S_OK)
				{
					if (hr == S_FALSE)
					{
						return DebuggerStatus::TimedOut;
					}

					return DebuggerStatus::Failed;
				}

				// Get the info about the last event.
				ULONG type;
				ULONG pid;
				ULONG tid;

				Tools::CheckHR(this->m_info->Control->GetLastEventInformation(
					&type,
					&pid,
					&tid,
					NULL,
					0,
					NULL,
					NULL,
					0,
					NULL
					));
				UNREFERENCED_PARAMETER(type);

				ULONG Status = DEBUG_STATUS_BREAK;

				for (int i = 0; i < m_lastArgs->Count; ++i)
				{
					DebuggerEventArgs^ lastArg = m_lastArgs[i];

					if (lastArg->GetType() == BreakpointEventArgs::typeid)
					{
						INVOKE(lastArg, BreakpointEventArgs, Breakpoint);
					}
					else if (lastArg->GetType() == CreateProcessEventArgs::typeid)
					{
						INVOKE(lastArg, CreateProcessEventArgs, ProcessCreated);
					}
					else if (lastArg->GetType() == CreateThreadEventArgs::typeid)
					{
						INVOKE(lastArg, CreateThreadEventArgs, ThreadCreated);
					}
					else if (lastArg->GetType() == ExceptionEventArgs::typeid)
					{
						INVOKE(lastArg, ExceptionEventArgs, Exception);
					}
					else if (lastArg->GetType() == ExitProcessEventArgs::typeid)
					{
						INVOKE(lastArg, ExitProcessEventArgs, ProcessExited);
					}
					else if (lastArg->GetType() == ExitThreadEventArgs::typeid)
					{
						INVOKE(lastArg, ExitThreadEventArgs, ThreadExited);
					}
					else if (lastArg->GetType() == SystemErrorEventArgs::typeid)
					{
						INVOKE(lastArg, SystemErrorEventArgs, SystemError);
					}
					else if (lastArg->GetType() == UnloadModuleEventArgs::typeid)
					{
						INVOKE(lastArg, UnloadModuleEventArgs, ModuleUnloaded);
					}

					if (Status == DEBUG_STATUS_BREAK || ((ULONG)lastArg->Status != DEBUG_STATUS_BREAK) && Status < (ULONG)lastArg->Status)
					{
						Status = (ULONG)lastArg->Status;
					}
				}
				m_lastArgs->Clear();

				if (Status == DEBUG_STATUS_BREAK)
				{
					// One more check.
					Status = DEBUG_STATUS_GO;
				}

				hr = this->m_info->Control->SetExecutionStatus(Status);
				if (!SUCCEEDED(hr))
				{
					throw gcnew System::InvalidOperationException("The debugger was broken into, but couldn't be resumed.");
				}

				return DebuggerStatus::Ok;
			}
#pragma endregion
#pragma region Output
			void Debuggee::Output(DbgEng::OutputFlags mmode, String ^mtext)
			{
				pin_ptr<const wchar_t> text = PtrToStringChars(mtext);
				Tools::CheckHR(this->m_info->Control->OutputWide((ULONG)mmode, text));
			}
#pragma endregion

#pragma region Modules
			DbgModule^ Debuggee::GetModuleByBase(ULONG64 moduleBase)
			{
				return gcnew DbgModule(this->m_info, moduleBase);
			}
			DbgModule^ Debuggee::GetModuleByName(String ^name)
			{
				ULONG64 base;
				ULONG index;
				this->m_info->DebugSymbols->GetModuleByModuleName(name, index, base);
				return gcnew DbgModule(this->m_info, base, index);
			}
			DbgModule^ Debuggee::GetModuleByIndex(ULONG index)
			{
				/*
				HRESULT
				  IDebugSymbols::GetModuleByIndex(
					IN ULONG  Index,
					OUT PULONG64  Base
					);
					*/
				ULONG64 base;
				Tools::CheckHR(this->m_info->Symbols->GetModuleByIndex(index, &base));
				return gcnew DbgModule(this->m_info, base, index);
			}

			ULONG Debuggee::ModuleCount::get()
			{
				/*
				HRESULT
				  IDebugSymbols::GetNumberModules(
					OUT PULONG  Loaded,
					OUT PULONG  Unloaded
					);
				*/
				ULONG nrLoaded;
				ULONG nrUnloaded;
				Tools::CheckHR(this->m_info->Symbols->GetNumberModules(&nrLoaded, &nrUnloaded));
				return nrLoaded + nrUnloaded;
			}

			array<DbgModule^>^ Debuggee::Modules::get()
			{
				/*
				HRESULT
				  IDebugSymbols::GetModuleParameters(
					IN ULONG  Count,
					IN OPTIONAL PULONG64  Bases,
					IN ULONG  Start,
					OUT PDEBUG_MODULE_PARAMETERS  Params
					);
				*/
				ULONG count = this->ModuleCount;

				PDEBUG_MODULE_PARAMETERS params = NULL;
				try
				{
					params = new _DEBUG_MODULE_PARAMETERS[count];
					Tools::CheckHR(this->m_info->Symbols->GetModuleParameters(count, NULL, 0, params));

					array<DbgModule^>^ result = gcnew array<DbgModule^>(count);
					for (ULONG i = 0;i < count;i++)
					{
						result[i] = gcnew DbgModule(this->m_info, params[i], i);
					}
					return result;
				}
				finally
				{
					delete[] params;
				}
			}
#pragma endregion

#pragma region Objects
			DbgObject^ Debuggee::GetDbgObject(ULONG64 offset, ULONG64 moduleBase, ULONG typeId)
			{
				return gcnew DbgObject(this->m_info, offset, moduleBase, typeId);
			}
			DbgObject^ Debuggee::GetDbgObject(ULONG64 offset, String^ module, String^ type)
			{
				ULONG64 moduleBase;
				ULONG moduleIndex;
				this->m_info->DebugSymbols->GetModuleByModuleName(module, moduleIndex, moduleBase);
				ULONG typeId = this->m_info->DebugSymbols->GetTypeId(moduleBase, type);
				return gcnew DbgObject(this->m_info, offset, moduleBase, typeId);
			}
#pragma endregion

			bool Debuggee::WaitForEvent(ULONG timeout)
			{
				return this->m_info->DebugControl->WaitForEvent(timeout);
			}
			//Not tested
			ULONG Debuggee::GetExitCode()
			{
				this->CheckDisposed();

				ULONG exitCode = 0;
				Tools::CheckHR(this->m_info->Client->GetExitCode(&exitCode));
				return exitCode;
			}
			DebugStatus Debuggee::GetExecutionStatus()
			{
				this->CheckDisposed();
				return this->m_info->DebugControl->GetExecutionStatus();
			}
			void Debuggee::Execute(String ^command)
			{
				this->CheckDisposed();
				return this->m_info->DebugControl->Execute(DebugOutputControl::ThisClient, command, DebugExecuteFlags::Echo | DebugExecuteFlags::NoRepeat);
			}

			ULONG Debuggee::ProcessCount::get()
			{
				return this->m_info->DebugSystemObjects->GetNumberProcesses();
			}
			String^ Debuggee::ProcessExecutableName::get()
			{
				return this->m_info->DebugSystemObjects->GetCurrentProcessExecutableName();
			}
			String^ Debuggee::ImagePath::get()
			{
				return this->m_info->DebugSymbols->GetImagePath();
			}
			void Debuggee::ImagePath::set(String^ value)
			{
				return this->m_info->DebugSymbols->SetImagePath(value);
			}

			DbgValue^ Debuggee::Evaluate(String ^expression)
			{
				//GetExpression("msvbvm60!g_itlsebthread");  
				return this->m_info->DebugControl->Evaluate(expression, DbgValueType::Invalid, NULL, DbgExpressionType::CPP);
			}
			void Debuggee::GetSymbolInfo(String ^symbol, [Runtime::InteropServices::Out] DbgType ^%type, [Runtime::InteropServices::Out] DbgModule ^%module)
			{
				ULONG typeId;
				ULONG64 moduleBase;
				this->m_info->DebugSymbols->GetSymbolType(symbol, typeId, moduleBase);
				type = gcnew DbgType(this->m_info, moduleBase, typeId);
				module = gcnew DbgModule(this->m_info, moduleBase);
			}
		}
	}
}

