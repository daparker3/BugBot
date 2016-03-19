#include "StdAfx.h"
#include "Tools.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			void Tools::ThrowDbgEngException(HRESULT hr)
			{
				Marshal::ThrowExceptionForHR(hr);
			}
			void Tools::CheckHR(HRESULT hr)
			{
				if(S_OK != hr)
					Tools::ThrowDbgEngException(hr);
			}

			LPCTSTR Tools::ConvertStringToLPCTSTR(String ^s)
			{
			    LPCTSTR lstr = 0;
			    //try
			    //{
				lstr = static_cast<LPCTSTR>(const_cast<void*>(static_cast<const void*>(Marshal::StringToHGlobalAuto(s))));
				// }
				// catch(ArgumentException ^e)
				// {
				//// handle the exception
				// }
				// catch (OutOfMemoryException ^e)
				// {
				//// handle the exception
				// }
				return lstr;
			}
			void Tools::FreeUnmanagedString(String ^managedString, LPCTSTR lstr)
			{
				if(!String::Compare(managedString, String::Empty))
				{
					Marshal::FreeHGlobal(static_cast<IntPtr>(const_cast<void*>(static_cast<const void*>(lstr))));
					lstr = 0;
				}
			}
		}
	}
}