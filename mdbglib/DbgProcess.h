#pragma once

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgProcess
			{
			private:
				DebuggeeInfo^ m_info;
				ULONG m_processId;

			internal:
				DbgProcess(DebuggeeInfo^ info, ULONG processId);
			};
		}
	}
}