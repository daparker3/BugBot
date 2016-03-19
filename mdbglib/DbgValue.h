#pragma once

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgValue : IConvertible, IDisposable
			{
			private:
				PDEBUG_VALUE _innerVal;
				bool disposed;
				!DbgValue();
			internal:
				DbgValue(_DEBUG_VALUE *legacyStruct);
				static void CreateLegacyStruct(Object ^value, PDEBUG_VALUE legacyStruct);
			public:
				~DbgValue();

				property DbgValueType Type
				{
					DbgValueType get()
					{
						return (DbgValueType)this->_innerVal->Type;
					}
				}

				virtual TypeCode GetTypeCode()
				{
					return System::Type::GetTypeCode(this->GetType());
				}

#pragma region IConvertable members
				virtual bool ToBoolean(IFormatProvider^);
				virtual BYTE ToByte(IFormatProvider^);
				virtual __wchar_t ToChar(IFormatProvider^);
				virtual DateTime ToDateTime(IFormatProvider^);
				virtual Decimal ToDecimal(IFormatProvider^);
				virtual Double ToDouble(IFormatProvider^);
				virtual SHORT ToInt16(IFormatProvider^);
				virtual int ToInt32(IFormatProvider^);
				virtual LONG64 ToInt64(IFormatProvider^);
				virtual signed char ToSByte(IFormatProvider^);
				virtual float ToSingle(IFormatProvider^);
				virtual String^ ToString(IFormatProvider^);
				virtual Object^ ToType(System::Type^, IFormatProvider^);
				virtual USHORT ToUInt16(IFormatProvider^);
				virtual UINT ToUInt32(IFormatProvider^);
				virtual ULONG64 ToUInt64(IFormatProvider^);
#pragma endregion

#pragma region Explicit casts
				static explicit operator byte(DbgValue ^value)
				{
					return value->ToByte(nullptr);
				}

				static explicit operator double(DbgValue ^value)
				{
					return value->ToDouble(nullptr);
				}

				static explicit operator float(DbgValue ^value)
				{
					return value->ToSingle(nullptr);
				}

				static explicit operator USHORT(DbgValue ^value)
				{
					return value->ToUInt16(nullptr);
				}

				static explicit operator UINT(DbgValue ^value)
				{
					return value->ToUInt32(nullptr);
				}

				static explicit operator ULONG64(DbgValue ^value)
				{
					return value->ToUInt64(nullptr);
				}

#pragma endregion

			};
		}
	}
}
