#pragma once

#include "stdafx.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugOutputEventArgs : EventArgs
			{
			private:
				DbgEng::OutputFlags m_outputFlags;
				String ^m_output;
			public:
				DebugOutputEventArgs(DbgEng::OutputFlags outputFlags, String ^output);
				property String ^Output 
				{
					String ^get();
				}
				property DbgEng::OutputFlags OutputFlags
				{
					DbgEng::OutputFlags get();
				}
			};
		}
	}
}
