#pragma once

using namespace System;
using namespace Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugSystemObjects
			{
			private:
				bool m_disposed;
				DbgSystemObjects* m_systemObjects;
			internal:

				DebugSystemObjects(DbgSystemObjects* systemObjects);
				~DebugSystemObjects();
				!DebugSystemObjects();

				void GetTotalNumberThreadsAndProcesses(PULONG pulTotalThreads, PULONG pulTotalProcesses, PULONG pulLargestProcessThreads, PULONG pulLargestSystemThreads, PULONG pulLargestSystemProcesses);

			public:
				ULONG GetEventProcessId();
				ULONG GetEventThreadId();
				ULONG GetNumberProcesses();
				ULONG GetProcessIdByDataOffset(ULONG64 offset);
				ULONG64 GetCurrentProcessDataOffset();
				String^ GetCurrentProcessExecutableName();
				ULONG GetProcessIdByHandle(ULONG64 handle);
				ULONG64 GetCurrentProcessHandle();
				ULONG GetCurrentProcessId();
				void SetCurrentProcessId(ULONG id);
				void GetProcessIdsByIndex(ULONG start, ULONG count,[Out] array<ULONG>^ %ids,[Out] array<ULONG>^ %sysIds);
				ULONG GetProcessIdByPeb(ULONG64 offset);
				ULONG64 GetCurrentProcessPeb();
				ULONG GetProcessIdBySystemId(ULONG sysId);
				ULONG GetCurrentProcessSystemId();
				ULONG GetNumberThreads();
				void GetTotalNumberThreads([Out] ULONG %totalThreads,[Out] ULONG %largestProcess);
				ULONG GetThreadIdByDataOffset(ULONG64 offset);
				ULONG64 GetCurrentThreadDataOffset();
				ULONG GetThreadIdByHandle(ULONG64 handle);
				ULONG64 GetCurrentThreadHandle();
				ULONG GetCurrentThreadId();
				void SetCurrentThreadId(ULONG id);
				void GetThreadIdsByIndex(ULONG start, ULONG count, [Out] array<ULONG>^ %ids,[Out] array<ULONG>^ %sysIds);
				ULONG GetThreadIdByProcessor(ULONG proc);
				ULONG GetThreadIdBySystemId(ULONG sysId);
				ULONG GetCurrentThreadSystemId();
				ULONG GetThreadIdByTeb(ULONG64 offset);
				ULONG64 GetCurrentThreadTeb();
				TimeSpan^ GetCurrentProcessUpTime();
				ULONG64 GetImplicitProcessDataOffset();
				void SetImplicitProcessDataOffset(ULONG64 offset);
				ULONG64 GetImplicitThreadDataOffset();
				void SetImplicitThreadDataOffset(ULONG64 offset);
				ULONG GetNumberSystems();
				ULONG GetEventSystem();
				void GetSystemIdsByIndex(ULONG start, ULONG count, [Out] array<ULONG>^ %ids); 
				ULONG GetCurrentSystemId();
				void SetCurrentSystemId(ULONG id);
				void GetTotalNumberThreadsAndProcesses([Out] ULONG %totalThreads, [Out] ULONG %totalProcesses, [Out] ULONG %largestProcessThreads, [Out] ULONG %largestSystemThreads, [Out] ULONG %largestSystemProcesses);
			};
		}
	}
}
