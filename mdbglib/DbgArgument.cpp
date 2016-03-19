#include "StdAfx.h"
#include "DbgArgument.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			String ^DbgArgument::Name::get()
			{
				return this->m_name;
			}
			void DbgArgument::Name::set(String ^value)
			{
				this->m_name = value;
			}
		}
	}
}
