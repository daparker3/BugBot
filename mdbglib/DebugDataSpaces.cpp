#include "StdAfx.h"
#include "DebugDataSpaces.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebugDataSpaces::DebugDataSpaces(DbgDataSpaces *dataSpaces)
			{
				this->m_dataSpaces = dataSpaces;
				this->m_disposed = false;
			}
			DebugDataSpaces::~DebugDataSpaces()
			{
				this->!DebugDataSpaces();
				this->m_disposed = true;
			}
			DebugDataSpaces::!DebugDataSpaces()
			{
			}

			String ^DebugDataSpaces::ReadUnicodeStringVirtual(ULONG64 offset, ULONG maxChars)
			{
				ULONG stringBytes;
				ULONG stringChars;
				ULONG maxBytes = maxChars * 2;
				HRESULT hr = this->m_dataSpaces->ReadUnicodeStringVirtualWide(offset, maxBytes, NULL, 0, &stringBytes);
				if(hr != S_FALSE) //Allow truncation
					Tools::CheckHR(hr);

				stringChars = stringBytes / 2;

				PWSTR pwResult = NULL;
				try
				{
					pwResult = new WCHAR[stringChars];
					hr = this->m_dataSpaces->ReadUnicodeStringVirtualWide(offset, stringBytes, pwResult, stringChars, NULL);
					if(hr != S_FALSE)
						Tools::CheckHR(hr);

					return gcnew String(pwResult);
				}
				finally
				{
					delete[] pwResult;
				}
			}
			String ^DebugDataSpaces::ReadTruncatedUnicodeStringVirtual(ULONG64 offset, ULONG maxChars)
			{
				System::Text::StringBuilder^ result = gcnew System::Text::StringBuilder();

				ULONG buffSize = 1024;
				ULONG buffChars = buffSize / 2;
				PWSTR buff = NULL;
				ULONG bytesRead;
				ULONG curChars = 0;
				try
				{
					buff = new WCHAR[buffChars + 1];
					buff[buffChars] = 0;
					ULONG endIndex = 0;
					do
					{
						bytesRead = 0;
						Tools::CheckHR(this->m_dataSpaces->ReadVirtual(offset, buff, buffSize, &bytesRead));
						ULONG charsRead = bytesRead / 2 + bytesRead % 2; //If there are more chars to read should be equal to buffChars
						for(endIndex = 0; endIndex<charsRead; endIndex++)
						{
							if(0 == buff[endIndex] || curChars == maxChars)
								break;
							curChars++;
						}
						offset+=bytesRead;
						buff[endIndex] = 0;
						result->Append(gcnew String(buff));
						if(endIndex < buffChars)
						{
							if(charsRead < buffChars)
								result->Append("<truncated, memory read error>");
							if(curChars == maxChars)
								result->Append("<truncated, maximum limit of " + maxChars.ToString() + " reached>");
						}
					}
					while((endIndex == buffChars) && (bytesRead == buffSize));
				}
				finally
				{
					delete[] buff;
				}

				return result->ToString();
			}

			String ^DebugDataSpaces::ReadTruncatedAnsiStringVirtual(ULONG64 offset, ULONG maxChars)
			{
				System::Text::StringBuilder^ result = gcnew System::Text::StringBuilder();

				ULONG buffSize = 1024;
				ULONG buffChars = buffSize;
				PSTR buff = NULL;
				ULONG bytesRead;
				ULONG curChars = 0;
				try
				{
					buff = new CHAR[buffChars + 1];
					buff[buffChars] = 0;
					ULONG endIndex = 0;
					do
					{
						bytesRead = 0;
						Tools::CheckHR(this->m_dataSpaces->ReadVirtual(offset, buff, buffSize, &bytesRead));
						ULONG charsRead = bytesRead; //If there are more chars to read should be equal to buffChars
						for(endIndex = 0; endIndex<charsRead; endIndex++)
						{
							if(0 == buff[endIndex] || curChars == maxChars)
								break;
							curChars++;
						}
						offset+=bytesRead;
						buff[endIndex] = 0;
						result->Append(System::Runtime::InteropServices::Marshal::PtrToStringAnsi(IntPtr(buff)));
						if(endIndex < buffChars)
						{
							if(charsRead < buffChars)
								result->Append("<truncated, memory read error>");
							if(curChars == maxChars)
								result->Append("<truncated, maximum limit of " + maxChars.ToString() + " reached>");
						}
					}
					while((endIndex == buffChars) && (bytesRead == buffSize));
				}
				finally
				{
					delete[] buff;
				}

				return result->ToString();
			}
		}
	}
}
