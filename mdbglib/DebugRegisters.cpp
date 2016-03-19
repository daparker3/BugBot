#include "StdAfx.h"
#include "DebugRegisters.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebugRegisters::DebugRegisters(DebuggeeInfo^ info)
			{
				m_info = info;
			}
		}
	}
}