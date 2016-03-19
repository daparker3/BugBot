#include "StdAfx.h"
#include "DebugAdvanced.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{

			DebugAdvanced::DebugAdvanced(DebuggeeInfo ^info)
			{
				this->m_info = info;
				this->m_disposed = false;
			}

			DebugAdvanced::~DebugAdvanced()
			{
				this->!DebugAdvanced();
				this->m_disposed = true;
			}
			DebugAdvanced::!DebugAdvanced()
			{
			}


		}
	}
}
