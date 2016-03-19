#pragma once

#include <msclr\lock.h>

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			ref class DebugClient;
			ref class DebugControl;
			ref class DebugSymbols;
			ref class DebugDataSpaces;
			ref class DebugSystemObjects;
			ref class DebugAdvanced;
			ref class DebugRegisters;

			private ref class DebuggeeInfo
			{
			private:
				bool disposed;

				DbgAdvanced *m_advanced;
				DbgClient *m_client;
				DbgControl *m_control;
				DbgDataSpaces *m_dataSpaces;
				DbgRegisters *m_registers;
				DbgSymbols *m_symbols;
				DbgSystemObjects *m_systemObjects;

				MS::Debuggers::DbgEng::DebugAdvanced^ m_debugAdvanced;
				MS::Debuggers::DbgEng::DebugClient^ m_debugClient;
				MS::Debuggers::DbgEng::DebugControl^ m_debugControl;
				MS::Debuggers::DbgEng::DebugDataSpaces^ m_debugDataSpaces;
				MS::Debuggers::DbgEng::DebugRegisters^ m_debugRegisters;
				MS::Debuggers::DbgEng::DebugSymbols^ m_debugSymbols;
				MS::Debuggers::DbgEng::DebugSystemObjects^ m_debugSystemObjects;
			internal:
				Object^ m_syncLock;

				void CheckDisposed()
				{
					if(this->disposed)
						throw gcnew System::ObjectDisposedException("DebuggeeInfo");
				}

				DebuggeeInfo(DbgClient *debugClient);
				~DebuggeeInfo(); //Dispose
				!DebuggeeInfo(); //Finalize

				property DbgClient *Client
				{
					DbgClient *get();
				}
				property DbgSymbols *Symbols
				{
					DbgSymbols *get();
				}
				property DbgControl *Control
				{
					DbgControl *get();
				}
				property DbgDataSpaces *DataSpaces
				{
					DbgDataSpaces *get();
				}
				property DbgSystemObjects *SystemObjects
				{
					DbgSystemObjects *get();
				}
				property DbgAdvanced *Advanced
				{
					DbgAdvanced *get();
				}
				property DbgRegisters *Registers
				{
					DbgRegisters *get();
				}

			public:

				property MS::Debuggers::DbgEng::DebugAdvanced^ DebugAdvanced
				{
					MS::Debuggers::DbgEng::DebugAdvanced^ get();
				}
				property MS::Debuggers::DbgEng::DebugClient^ DebugClient
				{
					MS::Debuggers::DbgEng::DebugClient^ get();
				}
				property MS::Debuggers::DbgEng::DebugControl^ DebugControl
				{
					MS::Debuggers::DbgEng::DebugControl^ get();
				}
				property MS::Debuggers::DbgEng::DebugDataSpaces^ DebugDataSpaces
				{
					MS::Debuggers::DbgEng::DebugDataSpaces^ get();
				}
				property MS::Debuggers::DbgEng::DebugRegisters^ DebugRegisters
				{
					MS::Debuggers::DbgEng::DebugRegisters^ get();
				}
				property MS::Debuggers::DbgEng::DebugSymbols^ DebugSymbols
				{
					MS::Debuggers::DbgEng::DebugSymbols^ get();
				}
				property MS::Debuggers::DbgEng::DebugSystemObjects^ DebugSystemObjects
				{
					MS::Debuggers::DbgEng::DebugSystemObjects^ get();
				}
			};
		}
	}
}