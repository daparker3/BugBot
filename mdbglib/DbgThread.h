#pragma once

#include "DebuggeeInfo.h"
#include "DbgFrame.h"

using namespace System;
using namespace System::Collections;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgThread
			{
			private:
				DebuggeeInfo^ m_info;
				ULONG m_threadId;
				ULONG m_threadSystemId;

			internal:
				DbgThread(DebuggeeInfo^ m_info, ULONG threadId, ULONG threadSystemId);
				~DbgThread();

			public:
				property unsigned int ThreadId
				{
					unsigned int get();
				}
				property unsigned int ThreadSystemId
				{
					unsigned int get();
				}
				array<DbgFrame^>^ GetStoredStackTrace(ULONG maxFrames);
			};
		}
	}
}