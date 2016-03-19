#pragma once

#include "stdafx.h"
#include "DebugOutputEventArgs.h"
#include "DbgThread.h"
#include "DbgProcess.h"
#include "DbgModule.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public enum class DebuggerStatus
			{
				Ok,
				Failed,
				TimedOut,
			};

			public ref class Debuggee : public System::IDisposable
			{
			private:
				ULONG64 m_procServer;
				DebuggeeInfo^ m_info;
				List<DebuggerEventArgs^>^ m_lastArgs;
				bool disposed;

				void CheckDisposed()
				{
					if (disposed)
						throw gcnew System::Exception("Debuggee object is disposed");
				}

#pragma region Events
				OutputCallbackWrapper *m_nativeOutCall;
				EventCallbackWrapper* m_nativeEventCall;
#pragma endregion

			internal:
#pragma region Events

#define DBGCAT(A,B) A ## B
#define DBGCAT2(A,B,C) A ## B ## C
#define HANDLER(N) DBGCAT2(m_,N,Handler)

#define EVTHANDLER(N,E) \
private: \
EventHandler<E^>^ DBGCAT2(m_,N,Handler); \
public: \
event EventHandler<E^>^ N \
{ \
	void add(EventHandler<E^>^ handler) \
	{ \
		this->HANDLER(N) = static_cast<EventHandler<E^>^>(Delegate::Combine(this->HANDLER(N), handler)); \
	} \
	void remove(EventHandler<E^>^ handler) \
	{ \
		this->HANDLER(N) = static_cast<EventHandler<E^>^>(Delegate::Remove(this->HANDLER(N), handler)); \
	} \
internal: \
	void raise(Object ^sender, E^ args) \
	{ \
		if(nullptr != this->HANDLER(N)) \
			this->HANDLER(N)(this, args); \
	} \
} \
internal: \

#define INVOKE(V,E,N) \
{ \
if(nullptr != HANDLER(N)) \
	this->HANDLER(N)(this,(E^)V); \
} \

#define DECLARE_EVENT_HANDLER(N,E,P,V) \
internal: \
DebugStatus DBGCAT(On,N) P \
{ \
E^ newEvt = gcnew E V; \
m_lastArgs->Add(newEvt); \
INVOKE(newEvt,E,DBGCAT(Pre,N)); \
return newEvt->Status; \
} \
EVTHANDLER(DBGCAT(Pre,N),E); \
EVTHANDLER(N,E); \

#define DECLARE_NONINTERRUPTABLE_EVENT_HANDLER(N,E,P,V) \
internal: \
DebugStatus DBGCAT(On,N) P \
{ \
E^ newEvt = gcnew E V; \
INVOKE(newEvt, E, N); \
return newEvt->Status; \
} \
EVTHANDLER(N,E); \

				EVTHANDLER(DebugOutput, DebugOutputEventArgs);

				void OnDebugOutput(OutputFlags flags, String^ output)
				{
					if (this->m_DebugOutputHandler != nullptr)
					{
						DebugOutputEventArgs^ evtArgs = gcnew DebugOutputEventArgs(flags, output);
						this->m_DebugOutputHandler(this, evtArgs);
					}
				}

				DECLARE_EVENT_HANDLER(Breakpoint, BreakpointEventArgs, (DbgBreak^ breakpoint), (breakpoint));
				DECLARE_EVENT_HANDLER(ProcessCreated, CreateProcessEventArgs, (\
				ULONG64 imageFileHandle, \
					ULONG64 handle, \
					ULONG64 baseOffset, \
					ULONG moduleSize, \
					String^ moduleName, \
					String^ imageName, \
					ULONG checksum, \
					ULONG timeDateStamp, \
					ULONG64 initialThreadHandle, \
					ULONG64 threadDataOffset, \
					ULONG64 startOffset \
					), \
					(\
						imageFileHandle, \
						handle, \
						baseOffset, \
						moduleSize, \
						moduleName, \
						imageName, \
						checksum, \
						timeDateStamp, \
						initialThreadHandle, \
						threadDataOffset, \
						startOffset \
						));
				DECLARE_EVENT_HANDLER(ThreadCreated, CreateThreadEventArgs, (\
				ULONG64 handle, \
					ULONG64 dataOffset, \
					ULONG64 startOffset \
					), \
					(\
						handle, \
						dataOffset, \
						startOffset \
						));
				DECLARE_EVENT_HANDLER(Exception, ExceptionEventArgs, (\
				NativeException^ exception, \
					bool firstChance \
					), \
					(\
						exception, \
						firstChance \
						));
				DECLARE_EVENT_HANDLER(ProcessExited, ExitProcessEventArgs, (ULONG exitCode), (exitCode));
				DECLARE_EVENT_HANDLER(ThreadExited, ExitThreadEventArgs, (ULONG exitCode), (exitCode));
				DECLARE_NONINTERRUPTABLE_EVENT_HANDLER(SessionStatus, SessionStatusEventArgs, (SessionStatusFlags flags), (flags));
				DECLARE_EVENT_HANDLER(SystemError, SystemErrorEventArgs, (ULONG error, ULONG level), (error, level));
				DECLARE_EVENT_HANDLER(ModuleLoaded, LoadModuleEventArgs, (\
				ULONG64 imageFileHandle, \
					ULONG64 baseOffset, \
					ULONG moduleSize, \
					String^ moduleName, \
					String^ imageName, \
					ULONG checkSum, \
					ULONG timeDateStamp \
					), (\
						imageFileHandle, \
						baseOffset, \
						moduleSize, \
						moduleName, \
						imageName, \
						checkSum, \
						timeDateStamp \
						));
				DECLARE_EVENT_HANDLER(ModuleUnloaded, UnloadModuleEventArgs, (String^ imageBase, ULONG64 baseOffset), (imageBase, baseOffset));

#pragma endregion Events

			protected:
				!Debuggee(); //Finalize method
			public:

				Debuggee();
				static Debuggee^ ConnectToProcessServer(String ^serverOptions);
				static Debuggee^ ConnectToKernel(String^ serverOptions);
				static Debuggee^ OpenDumpFile(System::String ^dumpFilePath, System::String ^symbolPath);
				~Debuggee(); //Dispose

				DebuggerStatus Update();

#pragma region Process enumeration
				String^ GetProcessName(ULONG processId, GetRunningProcessDescriptionFlags flags);
				ULONG GetSystemId(String^ processName, GetRunningProcessSystemIdByExecutableNameFlags flags);

				property array<ULONG>^ Processes
				{
					array<ULONG>^ get();
				}
#pragma endregion
#pragma region Symbols Location
				property String ^SymbolPath
				{
					String^ get();
					void set(String ^value);
				}
				void SymbolPathAppend(String ^symbolPath);
#pragma endregion
#pragma region Dump Files
				void OpenDumpFile(String ^dumpFilePath);
				void OpenDumpFile(IntPtr handle);
				void AddDumpFile(String ^dumpFilePath);
				void AddDumpFile(IntPtr handle);
				void WriteDumpFile(String ^path, DumpType type, DumpFlags flags, String ^comment);
				String ^GetDumpFile(ULONG index);
#pragma endregion
#pragma region Create/Attach/Detach
				void CreateProcess(String ^commandLine, String ^initialDirectory);
				void CreateAndAttachProcess(String ^commandLine, String ^initialDirectory);
				void AttachProcess(ULONG processId, AttachFlags flags);
				void DetachCurrentProcess();
				void DetachProcesses();
#pragma endregion
#pragma region Threads
				property array<DbgThread^>^ Threads
				{
					array<DbgThread^>^ get();
				}
				property ULONG EventThreadId
				{
					ULONG get();
				}
				property DbgThread^ EventThread
				{
					DbgThread^ get();
				}
				property ULONG EventProcessId
				{
					ULONG get();
				}
				property DbgProcess^ EventProcess
				{
					DbgProcess^ get();
				}
				property DbgProcess^ CurrentProcess
				{
					DbgProcess^ get();
				}
				property DbgThread^ CurrentThread
				{
					DbgThread^ get();
				}
				property ULONG CurrentThreadId
				{
					ULONG get();
					void set(ULONG threadId);
				}
				array<DbgFrame^>^ GetStoredStackTrace();
#pragma endregion
#pragma region Output
				void Output(OutputFlags flags, String ^text);
#pragma endregion
#pragma region Modules
				DbgModule^ GetModuleByBase(ULONG64 moduleBase);
				DbgModule^ GetModuleByName(String ^name);
				DbgModule^ GetModuleByIndex(ULONG index);
				property ULONG ModuleCount { ULONG get(); }
				property array<DbgModule^>^ Modules { array<DbgModule^>^ get(); }
#pragma endregion
#pragma region Objects
				DbgObject^ GetDbgObject(ULONG64 offset, ULONG64 moduleBase, ULONG typeId);
				DbgObject^ GetDbgObject(ULONG64 offset, String^ module, String^ type);
#pragma endregion
#pragma region Thin wrapper access
				property MS::Debuggers::DbgEng::DebugSymbols^ DebugSymbols
				{
					MS::Debuggers::DbgEng::DebugSymbols^ get() { return this->m_info->DebugSymbols; }
				}
				property MS::Debuggers::DbgEng::DebugSystemObjects^ DebugSystemObjects
				{
					MS::Debuggers::DbgEng::DebugSystemObjects^ get() { return this->m_info->DebugSystemObjects; }
				}
				property MS::Debuggers::DbgEng::DebugControl^ DebugControl
				{
					MS::Debuggers::DbgEng::DebugControl^ get() { return this->m_info->DebugControl; }
				}
				property MS::Debuggers::DbgEng::DebugDataSpaces^ DebugDataSpaces
				{
					MS::Debuggers::DbgEng::DebugDataSpaces^ get() { return this->m_info->DebugDataSpaces; }
				}
				property MS::Debuggers::DbgEng::DebugClient^ DebugClient
				{
					MS::Debuggers::DbgEng::DebugClient^ get() { return this->m_info->DebugClient; }
				}
#pragma endregion
				ULONG GetExitCode();
				DebugStatus GetExecutionStatus();
				void Execute(String ^command);
				property ULONG ProcessCount
				{
					ULONG get();
				}
				property String^ ProcessExecutableName
				{
					String^ get();
				}
				property ULONG CurrentProcessId
				{
					ULONG get();
					void set(ULONG value);
				}
				property String^ ImagePath
				{
					String^ get();
					void set(String^ value);
				}
				bool WaitForEvent(/*ULONG flags, */ULONG timeout); //No flags are currently supported
				DbgValue^ Evaluate(String ^expression);
				void GetSymbolInfo(String ^symbol, [Runtime::InteropServices::Out] DbgType ^%type, [Runtime::InteropServices::Out] DbgModule ^%module);
			};
		}
	}
}
