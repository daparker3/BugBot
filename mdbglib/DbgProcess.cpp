#include "StdAfx.h"
#include "DbgProcess.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgProcess::DbgProcess(DebuggeeInfo^ info, ULONG processId)
			{
				m_info = info;
				m_processId = processId;
			}
		}
	}
}