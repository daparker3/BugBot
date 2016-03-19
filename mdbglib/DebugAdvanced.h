#pragma once

#include "DbgFrame.h"
#include "DbgValue.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugAdvanced
			{
			private:
				DebuggeeInfo ^m_info;
				bool m_disposed;
			internal:
				DebugAdvanced(DebuggeeInfo^ info);
				!DebugAdvanced();
				~DebugAdvanced();

			public:

			};
		}
	}
}