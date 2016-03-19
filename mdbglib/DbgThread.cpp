#include "StdAfx.h"
#include "DbgThread.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgThread::DbgThread(DebuggeeInfo^ m_info, ULONG threadId, ULONG threadSystemId)
			{
				this->m_info = m_info;
				this->m_threadId = threadId;
				this->m_threadSystemId = threadSystemId;
			}

			DbgThread::~DbgThread()
			{
			}

			unsigned int DbgThread::ThreadId::get()
			{
				return this->m_threadId;
			}
			unsigned int DbgThread::ThreadSystemId::get()
			{
				return this->m_threadSystemId;
			}
			array<DbgFrame^>^ DbgThread::GetStoredStackTrace(ULONG maxFrames)
			{
				if(this->m_info->DebugSystemObjects->GetCurrentThreadId() != this->m_threadId)
					this->m_info->DebugSystemObjects->SetCurrentThreadId(this->m_threadId);

				array<DbgFrame^>^ result = this->m_info->DebugControl->GetStoredStackTrace(maxFrames);
				return result;
			}
		}
	}
}