#pragma once

using namespace System; 

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugClient
			{
			private:
				DbgClient *m_client;
				bool m_disposed;
			internal:
				DebugClient(DbgClient*);
				!DebugClient();
				~DebugClient();

			public:

#pragma region Dump Files
				void OpenDumpFile(String ^dumpFilePath, ULONG64 handle);
				void AddDumpFileInformation(String ^dumpFilePath, ULONG64 handle);
				void WriteDumpFile(String ^dumpFilePath, ULONG64 handle, ULONG qualifier, ULONG flags, String ^comment);
				String^ GetDumpFile(ULONG index);
				//String^ GetDumpFile(ULONG index, [Runtime::InteropServices::Out] IntPtr %ptrHandle);
#pragma endregion;
#pragma region Create/Attach/Detach
				void CreateProcess(ULONG64 server, String ^commandLine, DebugCreateProcessOptions options, String ^initialDirectory, String ^environment);
				void CreateProcess(String ^commandLine, String ^initialDirectory);
				void CreateProcessAndAttach(ULONG64 server, String ^commandLine, DebugCreateProcessOptions options, String ^initialDirectory, String ^environment, ULONG processId, AttachFlags flags);
				void CreateProcessAndAttach(String ^commandLine, String ^initialDirectory);
				void AttachProcess(ULONG64 server, ULONG processId, ULONG flags);
				void DetachProcesses();
				void DetachCurrentProcess();
#pragma endregion
				String^ GetIdentity();
				void OutputIdentity(DebugOutputControl control, String^ format);
			};
		}
	}
}