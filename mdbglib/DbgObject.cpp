#include "StdAfx.h"
#include "DbgObject.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgObject::DbgObject(DebuggeeInfo^ info, ULONG64 offset, ULONG64 moduleBase, ULONG typeId)
			{
				this->m_info = info;
				this->m_offset = offset;
				this->m_moduleBase = moduleBase;
				this->m_typeId = typeId;
			}

			bool DbgObject::IsNull::get()
			{
				return this->m_offset == 0;
			}

			DbgType^ DbgObject::Type::get()
			{
				if(nullptr == this->m_type)
					this->m_type = gcnew DbgType(this->m_info, this->m_moduleBase, this->m_typeId);

				return this->m_type;
			}

			DbgObject^ DbgObject::Dereference(String ^resultObjectType)
			{
				if(this->IsNull)
					throw gcnew NullReferenceException("Cannot dereference a object that points to a null");

				ULONG64 objectOffset;
				Tools::CheckHR(this->m_info->DataSpaces->ReadVirtual(this->m_offset, &objectOffset, sizeof(ULONG64), NULL));

				ULONG typeId = this->m_info->DebugSymbols->GetTypeId(this->m_moduleBase, resultObjectType);
				DbgObject^ obj = gcnew DbgObject(this->m_info, objectOffset, this->m_moduleBase, typeId);
				return obj;
			}

			DbgObject^ DbgObject::Dereference()
			{
				if (this->Type->Name->EndsWith("*"))
				{
					return this->Dereference(this->Type->Name->Substring(0, this->Type->Name->Length -1));
				}
				else
				{
					throw gcnew NotImplementedException("Cannot dereference a variable that is not of pointer type");
				}
			}

			DbgObject^ DbgObject::Cast(String ^typeName)
			{
				ULONG typeId = this->m_info->DebugSymbols->GetTypeId(this->m_moduleBase, typeName);
				return gcnew DbgObject(this->m_info, this->m_offset, this->m_moduleBase, typeId);
			}

			DbgObject^ DbgObject::ReadField(String ^fieldName)
			{
				ULONG fieldType;
				ULONG fieldOffset;

				this->m_info->DebugSymbols->GetFieldTypeAndOffset(this->m_moduleBase, this->m_typeId, fieldName, fieldType, fieldOffset);

				return gcnew DbgObject(this->m_info, this->m_offset + fieldOffset, this->m_moduleBase, fieldType);
			}

			String^ DbgObject::ReadAsStringUnicode(ULONG maxSize)
			{
				if(this->IsNull)
					throw gcnew NullReferenceException();

				//return DebugDataSpaces::ReadUnicodeStringVirtual(this->m_info->DataSpaces, this->m_offset, ULONG::MaxValue / 2); //
				//return DebugDataSpaces::ReadUnicodeStringVirtual(this->m_info->DataSpaces, this->m_offset, 768); //
				return this->m_info->DebugDataSpaces->ReadTruncatedUnicodeStringVirtual(this->m_offset, maxSize); //
			}
			String^ DbgObject::ReadAsStringASCII(ULONG maxSize)
			{
				if(this->IsNull)
					throw gcnew NullReferenceException();


				return this->m_info->DebugDataSpaces->ReadTruncatedAnsiStringVirtual(this->m_offset, maxSize); //
			}
		}
	}
}
