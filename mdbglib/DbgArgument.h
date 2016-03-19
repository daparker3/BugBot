#pragma once

#include "DbgObject.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgArgument : DbgObject
			{
			private:
				String ^m_name;
			internal:
				DbgArgument(DebuggeeInfo^ info, ULONG64 offset, ULONG64 moduleBase, ULONG typeId) : DbgObject(info, offset, moduleBase, typeId)
				{
				}
			public:
				property String ^Name
				{
					String ^get();
					void set(String ^value);
				}
			};
		}
	}
}