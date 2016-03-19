#pragma once

#include "DbgType.h"
using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgObject
			{
			internal:
				DebuggeeInfo^ m_info;
			private:
				DbgType^ m_type;
				ULONG64 m_offset;
				ULONG64 m_moduleBase;
				ULONG m_typeId;

			internal:
				DbgObject(DebuggeeInfo^ info, ULONG64 offset, ULONG64 moduleBase, ULONG typeId);

			public:
				property bool IsNull
				{
					bool get();
				}
				property DbgType^ Type
				{
					DbgType^ get();
				}
				DbgObject^ Dereference(String ^resultObjectType);
				DbgObject^ Dereference();
				DbgObject^ Cast(String ^typeName);
				DbgObject^ ReadField(String ^fieldName);
				String ^ReadAsStringUnicode()
				{
					return this->ReadAsStringUnicode(ULONG::MaxValue / 2);
				}
				String ^ReadAsStringASCII()
				{
					return this->ReadAsStringASCII(ULONG::MaxValue / 2);
				}
				String ^ReadAsStringUnicode(ULONG maxSize);
				String ^ReadAsStringASCII(ULONG maxSize);
			};
		}
	}
}
