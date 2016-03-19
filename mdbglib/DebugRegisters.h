#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugRegisters
			{
			private:
				DebuggeeInfo ^m_info;
			internal:
				DebugRegisters(DebuggeeInfo ^info);
			};
		}
	}
}