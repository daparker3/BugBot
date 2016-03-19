
#include "StdAfx.h"
#include "PCWSTRWrapper.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			PCWSTRWrapper::PCWSTRWrapper(String ^s)
			{
				try
				{
					if(!String::IsNullOrEmpty(s))
					{
						IntPtr^ ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s);
						this->_szPtr = ptr;
					}
				}
				catch(Exception^ e)
				{
					this->~PCWSTRWrapper();
					throw e;
				}
				this->disposed = false;
			}
			PCWSTRWrapper::~PCWSTRWrapper() //Dispose
			{
				//Clean managed resources here
				this->!PCWSTRWrapper();
				this->disposed = true;
				//No need to call SuppressFinalize, the compiler does it for you
			}
			PCWSTRWrapper::!PCWSTRWrapper() //Finalize
			{
				if (this->_szPtr->ToPointer() != NULL)
				{
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(*(this->_szPtr));
					this->_szPtr = IntPtr(0);
				}
			}
			IntPtr^ PCWSTRWrapper::SzPtr::get()
			{
				if (this->_szPtr->ToPointer() == 0)
				{
					return gcnew IntPtr(0);
				}
				return this->_szPtr;
			}
			PWSTR PCWSTRWrapper::GetUnmanagedString(String ^s)
			{
				return static_cast<PWSTR>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());
			}
			void PCWSTRWrapper::FreeUnmanagedString(PWSTR s)
			{
				if(NULL != s)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(IntPtr(s));
			}
		}
	}
}