#include "StdAfx.h"
#include "DbgType.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgType::DbgType(DebuggeeInfo^ info, ULONG64 moduleBase, ULONG typeId)
			{
				this->m_info = info;
				this->m_moduleBase = moduleBase;
				this->m_typeId = typeId;
			}

			String ^DbgType::Name::get()
			{
				if(nullptr == this->m_name)
				{
					PWSTR pwTypeName = NULL;
					try
					{
						ULONG nameSize;
						HRESULT hr = this->m_info->Symbols->GetTypeNameWide(this->m_moduleBase, this->m_typeId, NULL, 0, &nameSize);
						if (hr == -2147467259)
							this->m_name = "!Unknown";
						else
						{
							Tools::CheckHR(hr);

							pwTypeName = new WCHAR[nameSize];
							Tools::CheckHR(this->m_info->Symbols->GetTypeNameWide(this->m_moduleBase, this->m_typeId, pwTypeName, nameSize, NULL));

							this->m_name = gcnew String(pwTypeName);
						}
					}
					finally
					{
						delete[] pwTypeName;
					}
				}
				return this->m_name;
			}
			ULONG DbgType::Size::get()
			{
				if(!this->m_size.HasValue)
				{
					ULONG size;
					Tools::CheckHR(this->m_info->Symbols->GetTypeSize(this->m_moduleBase, this->m_typeId, &size));
					//Compiler bug? this->Size = size;
					this->m_size = Nullable<ULONG>(size);
				}
				return this->m_size.Value;
			}
			void DbgType::Size::set(ULONG value)
			{
				this->m_size = Nullable<ULONG>(value);
			}
		}
	}
}
