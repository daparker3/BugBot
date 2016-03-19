#pragma once

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgType
			{
			private:
				DebuggeeInfo^ m_info;
				ULONG64 m_moduleBase;
				ULONG m_typeId;
				String ^m_name;
				Nullable<ULONG> m_size;

			internal:
				DbgType(DebuggeeInfo^ info, ULONG64 moduleBase, ULONG typeId);
			public:
				property String ^Name
				{
					String ^get();
				}
				property ULONG Size
				{
					ULONG get();
				internal:
					void set(ULONG size);
				}
			};
		}
	}
}
