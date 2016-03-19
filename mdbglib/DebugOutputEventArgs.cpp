#include "StdAfx.h"
#include "DebugOutputEventArgs.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebugOutputEventArgs::DebugOutputEventArgs(DbgEng::OutputFlags outputFlags, String ^output)
			{
				m_outputFlags = outputFlags;
				m_output = output;
			}
			String ^DebugOutputEventArgs::Output::get()
			{
				return m_output;
			}
			DbgEng::OutputFlags DebugOutputEventArgs::OutputFlags::get()
			{
				return m_outputFlags;
			}
		}
	}
}