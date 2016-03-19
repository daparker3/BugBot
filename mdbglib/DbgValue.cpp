#include "StdAfx.h"
#include "DbgValue.h"

using namespace System;
using namespace System::Globalization;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgValue::DbgValue(_DEBUG_VALUE *pLegacyStruct)
			{
				if(NULL == pLegacyStruct)
					throw gcnew ArgumentNullException("pLegacyStruct");
				this->_innerVal = new _DEBUG_VALUE();
				*(this->_innerVal) = *pLegacyStruct;
				this->disposed = false;
			}

			void DbgValue::CreateLegacyStruct(Object ^value, PDEBUG_VALUE pLegacyStruct)
			{
				switch (System::Type::GetTypeCode(value->GetType()))
				{
					case TypeCode::Empty:
						pLegacyStruct->I8 = ((signed char) Convert::ToSByte(value, CultureInfo::CurrentCulture));
						pLegacyStruct->Type = 1;
						break;

					case TypeCode::Object:
						pLegacyStruct->I8 = Convert::ToByte(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 1;
						break;

					case TypeCode::DBNull:
						pLegacyStruct->I16 = ((USHORT) Convert::ToInt16(value, CultureInfo::CurrentCulture));
						pLegacyStruct->Type = 2;
						break;

					case TypeCode::Boolean:
						pLegacyStruct->I16 = Convert::ToUInt16(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 2;
						break;

					case TypeCode::Char:
						pLegacyStruct->I32 = Convert::ToInt32(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 3;
						break;

					case TypeCode::SByte:
						pLegacyStruct->I32 = Convert::ToUInt32(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 3;
						break;

					case TypeCode::Byte:
						pLegacyStruct->I64 = Convert::ToInt64(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 4;
						break;

					case TypeCode::Int16:
						pLegacyStruct->I64 = Convert::ToUInt64(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 4;
						break;

					case TypeCode::UInt16:
						pLegacyStruct->F32 = Convert::ToSingle(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 5;
						break;

					case TypeCode::Int32:
						pLegacyStruct->F64 = Convert::ToDouble(value, CultureInfo::CurrentCulture);
						pLegacyStruct->Type = 6;
						break;

					default:
						throw gcnew ArgumentException("Unsupported value type.", "value");
				}
			}

			DbgValue::~DbgValue() //Dispose
			{
				this->!DbgValue();
				this->disposed = true;
			}

			DbgValue::!DbgValue() //Finalizer
			{
				delete this->_innerVal;
			}

#pragma region IConvertable members
			bool DbgValue::ToBoolean(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			BYTE DbgValue::ToByte(IFormatProvider^)
			{
				return *(((byte*) this->_innerVal));
			}
			__wchar_t DbgValue::ToChar(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			DateTime DbgValue::ToDateTime(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			Decimal DbgValue::ToDecimal(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			Double DbgValue::ToDouble(IFormatProvider^)
			{
				return *(((double*) this->_innerVal));
			}
			SHORT DbgValue::ToInt16(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			int DbgValue::ToInt32(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			LONG64 DbgValue::ToInt64(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			signed char DbgValue::ToSByte(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			float DbgValue::ToSingle(IFormatProvider^)
			{
				return *(((float*) this->_innerVal));
			}
			String ^DbgValue::ToString(IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			Object^ DbgValue::ToType(System::Type^, IFormatProvider^)
			{
				throw gcnew InvalidCastException();
			}
			USHORT DbgValue::ToUInt16(IFormatProvider^)
			{
				return *(((USHORT*) this->_innerVal));
			}
			UINT DbgValue::ToUInt32(IFormatProvider^)
			{
				return *(((UINT*) this->_innerVal));
			}
			ULONG64 DbgValue::ToUInt64(IFormatProvider^)
			{
				return *(((ULONG*) this->_innerVal));
			}
#pragma endregion

		}
	}
}
