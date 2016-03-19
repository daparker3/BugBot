#include "StdAfx.h"
#include "DebugClient.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{

			DebugClient::DebugClient(DbgClient *client)
			{
				this->m_client = client;
				this->m_client->AddRef();
				this->m_disposed = false;
			}
			DebugClient::~DebugClient()
			{
				try
				{
					this->!DebugClient();
				}
				finally
				{
					this->m_disposed = true;
				}
			}
			DebugClient::!DebugClient()
			{
				if(NULL != this->m_client)
					this->m_client->Release();
			}



#pragma region Dump Files

			void DebugClient::OpenDumpFile(String ^dumpFilePath, ULONG64 handle)
			{
//HRESULT
//  IDebugClient4::OpenDumpFileWide(
//    IN OPTIONAL PCWSTR  FileName,
//    IN ULONG64  FileHandle
//    );
				pin_ptr<const wchar_t> pDumpFilePath = nullptr;
				if(!String::IsNullOrEmpty(dumpFilePath))
					pDumpFilePath = PtrToStringChars(dumpFilePath);
				Tools::CheckHR(this->m_client->OpenDumpFileWide(pDumpFilePath, handle));
			}

			void DebugClient::AddDumpFileInformation(String ^dumpFilePath, ULONG64 handle)
			{
//HRESULT
//  IDebugClient4::AddDumpInformationFileWide(
//    IN OPTIONAL PCWSTR  FileName,
//    IN ULONG64  FileHandle,
//    IN ULONG  Type
//    );
				pin_ptr<const wchar_t> pDumpFilePath = nullptr;
				if(!String::IsNullOrEmpty(dumpFilePath))
					pDumpFilePath = PtrToStringChars(dumpFilePath);

				Tools::CheckHR(this->m_client->AddDumpInformationFileWide(pDumpFilePath, handle, DEBUG_DUMP_FILE_PAGE_FILE_DUMP));
			}
			void DebugClient::WriteDumpFile(String ^dumpFilePath, ULONG64 handle, ULONG qualifier, ULONG flags, String ^comment)
			{
//HRESULT
//  IDebugClient4::WriteDumpFileWide(
//    IN OPTIONAL PCWSTR  FileName,
//    IN ULONG64  FileHandle,
//    IN ULONG  Qualifier,
//    IN ULONG  FormatFlags,
//    IN OPTIONAL PCWSTR  Comment
//    );
				pin_ptr<const wchar_t> pDumpFilePath = PtrToStringChars(dumpFilePath);
				pin_ptr<const wchar_t> pComment = PtrToStringChars(comment);
				Tools::CheckHR(this->m_client->WriteDumpFileWide(pDumpFilePath, handle, qualifier, flags, pComment));
			}
			String^ DebugClient::GetDumpFile(ULONG index)
			{
				ULONG ulNameSize = 0;
				ULONG ulType = 0;
				PWSTR pwFileName = NULL;
				try
				{
					Tools::CheckHR(this->m_client->GetDumpFileWide(index, NULL, NULL, &ulNameSize, NULL, &ulType));
					pwFileName = new WCHAR[ulNameSize];
					Tools::CheckHR(this->m_client->GetDumpFileWide(index, pwFileName, ulNameSize, NULL, NULL, &ulType));
					return gcnew System::String(pwFileName);
				}
				finally
				{
					delete[] pwFileName;
				}
			}
			//String^ DebugClient::GetDumpFile(ULONG index, [Runtime::InteropServices::Out] IntPtr %ptrHandle)
			//{
			//	ULONG ulNameSize = 0;
			//	ULONG ulType = 0;
			//	ULONG64 handle = 0;
			//	PWSTR pwFileName = NULL;
			//	try
			//	{
			//		Tools::CheckHR(this->m_client->GetDumpFileWide(index, NULL, NULL, &ulNameSize, &handle, &ulType));
			//		pwFileName = new WCHAR[ulNameSize];
			//		Tools::CheckHR(this->m_client->GetDumpFileWide(index, pwFileName, ulNameSize, NULL, NULL, &ulType));
			//		ptrHandle = IntPtr((void *)handle); //TODO: Test
			//		return gcnew System::String(pwFileName);
			//	}
			//	finally
			//	{
			//			delete[] pwFileName;
			//	}
			//}

#pragma endregion
		
#pragma region Create/Attach/Detach
			void DebugClient::CreateProcess(ULONG64 server, String ^commandLine, DebugCreateProcessOptions options, String ^initialDirectory, String ^environment)
			{
//HRESULT
//  IDebugClient5::CreateProcess2Wide(
//    IN ULONG64  Server,
//    IN PWSTR  CommandLine,
//    IN PVOID  OptionsBuffer,
//    IN ULONG  OptionsBufferSize,
//    IN OPTIONAL PCWSTR  InitialDirectory,
//    IN OPTIONAL PCWSTR  Environment
//    );
				pin_ptr<const wchar_t> pCommandLine = PtrToStringChars(commandLine);
				pin_ptr<const wchar_t> pInitialDirectory = PtrToStringChars(initialDirectory);
				pin_ptr<const wchar_t> pEnvironment = PtrToStringChars(environment);
				Tools::CheckHR(this->m_client->CreateProcess2Wide(server, (PWSTR)pCommandLine, &options.ToLegacy(), sizeof(DEBUG_CREATE_PROCESS_OPTIONS), pInitialDirectory, pEnvironment));
			}
			void DebugClient::CreateProcess(String ^commandLine, String ^initialDirectory)
			{
				DebugCreateProcessOptions options;
				//TODO: Init default values
				this->CreateProcess(0, commandLine, options, initialDirectory, nullptr);
			}
			void DebugClient::CreateProcessAndAttach(ULONG64 server, String ^commandLine, DebugCreateProcessOptions options, String ^initialDirectory, String ^environment, ULONG processId, AttachFlags flags)
			{

//HRESULT
//  IDebugClient5::CreateProcessAndAttach2Wide(
//    IN ULONG64  Server,
//    IN OPTIONAL PWSTR  CommandLine,
//    IN PVOID  OptionsBuffer,
//    IN ULONG  OptionsBufferSize,
//    IN OPTIONAL PCWSTR  InitialDirectory,
//    IN OPTIONAL PCWSTR  Environment,
//    IN ULONG  ProcessId,
//    IN ULONG  AttachFlags
//    );
				pin_ptr<const wchar_t> pCommandLine = PtrToStringChars(commandLine);
				pin_ptr<const wchar_t> pInitialDirectory = PtrToStringChars(initialDirectory);
				pin_ptr<const wchar_t> pEnvironment = PtrToStringChars(environment);
				Tools::CheckHR(this->m_client->CreateProcessAndAttach2Wide(server, (PWSTR)pCommandLine, &options.ToLegacy(), sizeof(DEBUG_CREATE_PROCESS_OPTIONS), pInitialDirectory, pEnvironment, processId, (ULONG)flags));
			}
			void DebugClient::CreateProcessAndAttach(String ^commandLine, String ^initialDirectory)
			{
				DebugCreateProcessOptions options;
				options.CreateFlags = MS::Debuggers::DbgEng::CreateFlags::DebugProcess;
				//TODO: Init default values
				this->CreateProcessAndAttach(0, commandLine, options, initialDirectory, nullptr, 0, AttachFlags::InvasiveNoInitialBreak);
			}
			void DebugClient::AttachProcess(ULONG64 server, ULONG processId, ULONG flags)
			{
				Tools::CheckHR(this->m_client->AttachProcess(server, processId, flags));
			}
			void DebugClient::DetachProcesses()
			{
				Tools::CheckHR(this->m_client->DetachProcesses());
			}
			void DebugClient::DetachCurrentProcess()
			{
				Tools::CheckHR(this->m_client->DetachCurrentProcess());
			}
#pragma endregion
			String^ DebugClient::GetIdentity()
			{
//HRESULT
//  IDebugClient5::GetIdentityWide(
//    OUT OPTIONAL PWSTR  Buffer,
//    IN ULONG  BufferSize,
//    OUT OPTIONAL PULONG  IdentitySize
//    );
				PWSTR buff = NULL;
				try
				{
					ULONG identitySize;
					Tools::CheckHR(m_client->GetIdentityWide(NULL, 0, &identitySize));
					buff = new WCHAR[identitySize];
					Tools::CheckHR(m_client->GetIdentityWide(buff, identitySize, NULL));
					return gcnew String(buff);
				}
				finally
				{
					delete[] buff;
				}
			}
			void DebugClient::OutputIdentity(DebugOutputControl control, String^ format)
			{
//HRESULT
//  IDebugClient5::OutputIdentityWide(
//    IN ULONG  OutputControl,
//    IN ULONG  Flags,
//    IN PCWSTR  Format
//    );
				pin_ptr<const wchar_t> pFormat = PtrToStringChars(format);
				Tools::CheckHR(m_client->OutputIdentityWide((ULONG) control, 0, pFormat));
			}
		}

	}
}