#include "StdAfx.h"
#include "DebugSystemObjects.h"
#include "Tools.h"

using namespace System;
using namespace Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebugSystemObjects::DebugSystemObjects(DbgSystemObjects* systemObjects)
			{
				this->m_systemObjects = systemObjects;
				this->m_systemObjects->AddRef();
				this->m_disposed = false;
			}
			DebugSystemObjects::~DebugSystemObjects()
			{
				this->!DebugSystemObjects();
				this->m_disposed = true;
			}
			DebugSystemObjects::!DebugSystemObjects()
			{
				if(NULL != this->m_systemObjects)
				{
					this->m_systemObjects->Release();
					this->m_systemObjects = NULL;
				}
			}

			ULONG DebugSystemObjects::GetEventProcessId()
			{
//HRESULT
//  IDebugSystemObjects::GetEventProcess(
//    OUT PULONG  Id
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetEventProcess(&result));
				return result;
			}

			ULONG DebugSystemObjects::GetEventThreadId()
			{
//HRESULT
//  IDebugSystemObjects::GetEventThread(
//    OUT PULONG  Id
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetEventThread(&result));
				return result;
			}

			ULONG DebugSystemObjects::GetNumberProcesses()
			{
//HRESULT
//  IDebugSystemObjects::GetNumberProcesses(
//    OUT PULONG  Number
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetNumberProcesses(&result));
				return result;
			}

			ULONG DebugSystemObjects::GetProcessIdByDataOffset(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects::GetProcessIdByDataOffset(
//    IN ULONG64  Offset,
//    OUT PULONG  Id
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetProcessIdByDataOffset(offset, &result));
				return result;
			}

			ULONG64 DebugSystemObjects::GetCurrentProcessDataOffset()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentProcessDataOffset(
//    OUT PULONG64  Offset
//    );
				ULONG64 result;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessDataOffset(&result));
				return result;
			}

			String^ DebugSystemObjects::GetCurrentProcessExecutableName()
			{
//HRESULT
//  IDebugSystemObjects4::GetCurrentProcessExecutableNameWide(
//    OUT OPTIONAL PWSTR  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  ExeSize
//    );
				PWSTR pwExecutableName = NULL;
				try
				{
					ULONG nameSize;
					Tools::CheckHR(this->m_systemObjects->GetCurrentProcessExecutableNameWide(NULL, 0, &nameSize));
					pwExecutableName = new WCHAR[nameSize];
					Tools::CheckHR(this->m_systemObjects->GetCurrentProcessExecutableNameWide(pwExecutableName, nameSize, NULL));
					return gcnew String(pwExecutableName);
				}
				finally
				{
					delete[] pwExecutableName;
				}
			}

			ULONG DebugSystemObjects::GetProcessIdByHandle(ULONG64 handle)
			{
//HRESULT
//  IDebugSystemObjects::GetProcessIdByHandle(
//    IN ULONG64  Handle,
//    OUT PULONG  Id
//    );
				ULONG procId;
				Tools::CheckHR(this->m_systemObjects->GetProcessIdByHandle(handle, &procId));
				return procId;
			}

			ULONG64 DebugSystemObjects::GetCurrentProcessHandle()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentProcessHandle(
//    OUT PULONG64  Handle
//    );
				ULONG64 hProc;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessHandle(&hProc));
				return hProc;
			}
			ULONG DebugSystemObjects::GetCurrentProcessId()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentProcessId(
//    OUT PULONG  Id
//    );
				ULONG procId;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessId(&procId));
				return procId;
			}
			void DebugSystemObjects::SetCurrentProcessId(ULONG procId)
			{
//HRESULT
//  IDebugSystemObjects::SetCurrentProcessId(
//    IN ULONG  Id
//    );
				Tools::CheckHR(this->m_systemObjects->SetCurrentProcessId(procId));
			}
			void DebugSystemObjects::GetProcessIdsByIndex(ULONG start, ULONG count, [Out] array<ULONG>^ %ids, [Out] array<ULONG>^ %sysIds)
			{
//HRESULT
//  IDebugSystemObjects::GetProcessIdsByIndex(
//    IN ULONG  Start,
//    IN ULONG  Count,
//    OUT OPTIONAL PULONG  Ids,
//    OUT OPTIONAL PULONG  SysIds
//    );
				ids = gcnew array<ULONG>(count);
				sysIds = gcnew array<ULONG>(count);
				pin_ptr<ULONG> pIds = &ids[0];
				pin_ptr<ULONG> pSysIds = &sysIds[0];
				Tools::CheckHR(this->m_systemObjects->GetProcessIdsByIndex(start, count, pIds, pSysIds));
			}
			ULONG DebugSystemObjects::GetProcessIdByPeb(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects::GetProcessIdByPeb(
//    IN ULONG64  Offset,
//    OUT PULONG  Id
//    );
				ULONG procId;
				Tools::CheckHR(this->m_systemObjects->GetProcessIdByPeb(offset, &procId));
				return procId;
			}
			ULONG64 DebugSystemObjects::GetCurrentProcessPeb()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentProcessPeb(
//    OUT PULONG64  Offset
//    );
				ULONG64 offset;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessPeb(&offset));
				return offset;
			}
			ULONG DebugSystemObjects::GetProcessIdBySystemId(ULONG sysId)
			{
//HRESULT
//  IDebugSystemObjects::GetProcessIdBySystemId(
//    IN ULONG  SysId,
//    OUT PULONG  Id
//    );
				ULONG procId;
				Tools::CheckHR(this->m_systemObjects->GetProcessIdBySystemId(sysId, &procId));
				return procId;
			}
			ULONG DebugSystemObjects::GetCurrentProcessSystemId()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentProcessSystemId(
//   OUT PULONG  SysId
//    );
				ULONG sysId;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessSystemId(&sysId));
				return sysId;
			}
			ULONG DebugSystemObjects::GetNumberThreads()
			{
//HRESULT
//  IDebugSystemObjects::GetNumberThreads(
//    OUT PULONG  Number
//    );
				ULONG number;
				Tools::CheckHR(this->m_systemObjects->GetNumberThreads(&number));
				return number;
			}
			void DebugSystemObjects::GetTotalNumberThreads([Out] ULONG %totalThreads,[Out] ULONG %largestProcess)
			{
//HRESULT
//  IDebugSystemObjects::GetTotalNumberThreads(
//    OUT PULONG  Total,
//    OUT PULONG  LargestProcess
//    );
				ULONG mTotalThreads;
				ULONG mLargestProcess;
				Tools::CheckHR(this->m_systemObjects->GetTotalNumberThreads(&mTotalThreads, &mLargestProcess));
				totalThreads = mTotalThreads;
				largestProcess = mLargestProcess;
			}
			ULONG DebugSystemObjects::GetThreadIdByDataOffset(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdByDataOffset(
//    IN ULONG64  Offset,
//    OUT PULONG  Id
//    );
				ULONG threadId;
				Tools::CheckHR(this->m_systemObjects->GetThreadIdByDataOffset(offset, &threadId));
				return threadId;
			}
			ULONG64 DebugSystemObjects::GetCurrentThreadDataOffset()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentThreadDataOffset(
//    OUT PULONG64  Offset
//    );
				ULONG64 offset;
				Tools::CheckHR(this->m_systemObjects->GetCurrentThreadDataOffset(&offset));
				return offset;
			}
			ULONG DebugSystemObjects::GetThreadIdByHandle(ULONG64 handle)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdByHandle(
//    IN ULONG64  Handle,
//    OUT PULONG  Id
//    );
				ULONG threadId;
				Tools::CheckHR(this->m_systemObjects->GetThreadIdByHandle(handle, &threadId));
				return threadId;
			}
			ULONG64 DebugSystemObjects::GetCurrentThreadHandle()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentThreadHandle(
//    OUT PULONG64  Handle
//    );
				ULONG64 handle;
				Tools::CheckHR(this->m_systemObjects->GetCurrentThreadHandle(&handle));
				return handle;
			}
			ULONG DebugSystemObjects::GetCurrentThreadId()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentThreadId(
//    OUT PULONG  Id
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetCurrentThreadId(&result));
				return result;
			}
			void DebugSystemObjects::SetCurrentThreadId(ULONG id)
			{
//HRESULT
//  IDebugSystemObjects::SetCurrentThreadId(
//	IN ULONG  Id
//	);
				Tools::CheckHR(this->m_systemObjects->SetCurrentThreadId(id));
			}
			void DebugSystemObjects::GetThreadIdsByIndex(ULONG start, ULONG count, [Out] array<ULONG>^ %ids,[Out] array<ULONG>^ %sysIds)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdsByIndex(
//    IN ULONG  Start,
//    IN ULONG  Count,
//    OUT OPTIONAL PULONG  Ids,
//    OUT OPTIONAL PULONG  SysIds
//    );
					ids = gcnew array<ULONG>(count);
					sysIds = gcnew array<ULONG>(count);
					pin_ptr<ULONG> pIds = &ids[0];
					pin_ptr<ULONG> pSysIds = &sysIds[0];
					Tools::CheckHR(this->m_systemObjects->GetThreadIdsByIndex(start, count, pIds, pSysIds));
			}
			ULONG DebugSystemObjects::GetThreadIdByProcessor(ULONG proc)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdByProcessor(
//    IN ULONG  Processor,
//    OUT PULONG  Id
//    );
				ULONG threadId;
				Tools::CheckHR(this->m_systemObjects->GetThreadIdByProcessor(proc,&threadId));
				return threadId;
			}
			ULONG DebugSystemObjects::GetThreadIdBySystemId(ULONG sysId)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdBySystemId(
//    IN ULONG  SysId,
//    OUT PULONG  Id
//    );
				ULONG threadId;
				Tools::CheckHR(this->m_systemObjects->GetThreadIdBySystemId(sysId,&threadId));
				return threadId;
			}
			ULONG DebugSystemObjects::GetCurrentThreadSystemId()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentThreadSystemId(
//    OUT PULONG  SysId
//    );
				ULONG result;
				Tools::CheckHR(this->m_systemObjects->GetCurrentThreadSystemId(&result));
				return result;
			}
			ULONG DebugSystemObjects::GetThreadIdByTeb(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects::GetThreadIdByTeb(
//    IN ULONG64  Offset,
//    OUT PULONG  Id
//    );
				ULONG thradId;
				Tools::CheckHR(this->m_systemObjects->GetThreadIdByTeb(offset, &thradId));
				return thradId;
			}
			ULONG64 DebugSystemObjects::GetCurrentThreadTeb()
			{
//HRESULT
//  IDebugSystemObjects::GetCurrentThreadTeb(
//    OUT PULONG64  Offset
//    );
				ULONG64 offset;
				Tools::CheckHR(this->m_systemObjects->GetCurrentThreadTeb(&offset));
				return offset;
			}
			TimeSpan^ DebugSystemObjects::GetCurrentProcessUpTime()
			{
//HRESULT
//  IDebugSystemObjects2::GetCurrentProcessUpTime(
//    OUT PULONG  UpTime
//    );
				ULONG sec;
				Tools::CheckHR(this->m_systemObjects->GetCurrentProcessUpTime(&sec));
				return gcnew TimeSpan(0, 0, (Int32)sec);
			}
			ULONG64 DebugSystemObjects::GetImplicitProcessDataOffset()
			{
//HRESULT
//  IDebugSystemObjects2::GetImplicitProcessDataOffset(
//    OUT PULONG64  Offset
//    );
				ULONG64 offset;
				Tools::CheckHR(this->m_systemObjects->GetImplicitProcessDataOffset(&offset));
				return offset;
			}
			void DebugSystemObjects::SetImplicitProcessDataOffset(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects2::SetImplicitProcessDataOffset(
//    IN ULONG64  Offset
//    );
				Tools::CheckHR(this->m_systemObjects->SetImplicitProcessDataOffset(offset));
			}
			ULONG64 DebugSystemObjects::GetImplicitThreadDataOffset()
			{
//HRESULT
//  IDebugSystemObjects2::GetImplicitThreadDataOffset(
//    OUT PULONG64  Offset
//    );
				ULONG64 offset;
				Tools::CheckHR(this->m_systemObjects->GetImplicitThreadDataOffset(&offset));
				return offset;
			}
			void DebugSystemObjects::SetImplicitThreadDataOffset(ULONG64 offset)
			{
//HRESULT
//  IDebugSystemObjects2::SetImplicitThreadDataOffset(
//    IN ULONG64  Offset
//    );
				Tools::CheckHR(this->m_systemObjects->SetImplicitThreadDataOffset(offset));
			}
			ULONG DebugSystemObjects::GetNumberSystems()
			{
//HRESULT
//  IDebugSystemObjects3::GetNumberSystems(
//    OUT PULONG  Number
//    );
				ULONG number;
				Tools::CheckHR(this->m_systemObjects->GetNumberSystems(&number));
				return number;
			}
			ULONG DebugSystemObjects::GetEventSystem()
			{
//HRESULT
//  IDebugSystemObjects3::GetEventSystem(
//    OUT PULONG  Id
//    );
				ULONG id;
				Tools::CheckHR(this->m_systemObjects->GetEventSystem(&id));
				return id;
			}
			void DebugSystemObjects::GetSystemIdsByIndex(ULONG start, ULONG count, [Out] array<ULONG>^ %ids)
			{
//HRESULT
//  IDebugSystemObjects3::GetSystemIdsByIndex(
//    IN ULONG  Start,
//    IN ULONG  Count,
//    OUT PULONG  Ids
//    );
				ids = gcnew array<ULONG>(count);
				pin_ptr<ULONG> pIds = &ids[0];
				Tools::CheckHR(this->m_systemObjects->GetSystemIdsByIndex(start, count, pIds));
			}
			ULONG DebugSystemObjects::GetCurrentSystemId()
			{
//HRESULT
//  IDebugSystemObjects3::GetCurrentSystemId(
//    OUT PULONG  Id
//    );
				ULONG id;
				Tools::CheckHR(this->m_systemObjects->GetCurrentSystemId(&id));
				return id;
			}
			void DebugSystemObjects::SetCurrentSystemId(ULONG id)
			{
//HRESULT
//  IDebugSystemObjects3::SetCurrentSystemId(
//    IN ULONG  Id
//    );
				Tools::CheckHR(this->m_systemObjects->SetCurrentSystemId(id));
			}
			void DebugSystemObjects::GetTotalNumberThreadsAndProcesses([Out] ULONG %totalThreads, [Out] ULONG %totalProcesses, [Out] ULONG %largestProcessThreads, [Out] ULONG %largestSystemThreads, [Out] ULONG %largestSystemProcesses)
			{
//HRESULT
//  IDebugSystemObjects3::GetTotalNumberThreadsAndProcesses(
//    OUT PULONG  TotalThreads,
//    OUT PULONG  TotalProcesses,
//    OUT PULONG  LargestProcessThreads,
//    OUT PULONG  LargestSystemThreads,
//    OUT PULONG  LargestSystemProcesses
//    );
				ULONG  mTotalThreads;
				ULONG  mTotalProcesses;
				ULONG  mLargestProcessThreads;
				ULONG  mLargestSystemThreads;
				ULONG  mLargestSystemProcesses;
				Tools::CheckHR(this->m_systemObjects->GetTotalNumberThreadsAndProcesses(&mTotalThreads, &mTotalProcesses, &mLargestProcessThreads, &mLargestSystemThreads, &mLargestSystemProcesses));
				totalThreads = mTotalThreads;
				totalProcesses = mTotalProcesses;
				largestProcessThreads = mLargestProcessThreads;
				largestSystemThreads = mLargestSystemThreads;
				largestSystemProcesses = mLargestSystemProcesses;
			}
			void DebugSystemObjects::GetTotalNumberThreadsAndProcesses(PULONG pulTotalThreads, PULONG pulTotalProcesses, PULONG pulLargestProcessThreads, PULONG pulLargestSystemThreads, PULONG pulLargestSystemProcesses)
			{
				Tools::CheckHR(this->m_systemObjects->GetTotalNumberThreadsAndProcesses(pulTotalThreads, pulTotalProcesses, pulLargestProcessThreads, pulLargestSystemThreads, pulLargestSystemProcesses));
			}
		}
	}
}
