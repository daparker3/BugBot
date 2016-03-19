#include "StdAfx.h"
#include "DebuggeeInfo.h"

using namespace System;
using namespace System::Threading;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DebuggeeInfo::DebuggeeInfo(DbgClient *debugClient) : m_syncLock(gcnew Object())
			{
				if(NULL == debugClient)
					throw gcnew System::ArgumentNullException("debugClient");
				this->m_client = debugClient;
				this->disposed = false;

				this->m_advanced		= NULL;
				//No m_client here
				this->m_control			= NULL;
				this->m_dataSpaces		= NULL;
				this->m_registers		= NULL;
				this->m_symbols			= NULL;
				this->m_systemObjects	= NULL;

				this->m_debugAdvanced		= nullptr;
				this->m_debugClient			= nullptr;
				this->m_debugControl		= nullptr;
				this->m_debugDataSpaces		= nullptr;
				this->m_debugRegisters		= nullptr;
				this->m_debugSymbols		= nullptr;
				this->m_debugSystemObjects	= nullptr;
			}

			DebuggeeInfo::~DebuggeeInfo() //m_debugAdvanced
			{
				this->!DebuggeeInfo();
				this->disposed = true;
				//No need to call SuppressFinalize, the compiler does it for you
			}

			DebuggeeInfo::!DebuggeeInfo() //Finalize
			{
				if(NULL != this->m_advanced)
					this->m_advanced->Release();
				if(NULL != this->m_client)
					this->m_client->Release();
				if(NULL != this->m_control)
					this->m_control->Release();
				if(NULL != this->m_dataSpaces)
					this->m_dataSpaces->Release();
				if(NULL != this->m_registers)
					this->m_registers->Release();
				if(NULL != this->m_symbols)
					this->m_symbols->Release();
				if(NULL != this->m_systemObjects)
					this->m_systemObjects->Release();

				//Setting to NULL so that anybody who references crashes right away
				this->m_advanced = NULL;
				this->m_client = NULL;
				this->m_control = NULL;
				this->m_dataSpaces = NULL;
				this->m_registers = NULL;
				this->m_symbols = NULL;
				this->m_systemObjects = NULL;
			}

			DbgAdvanced *DebuggeeInfo::Advanced::get()
			{
				this->CheckDisposed();
				if(NULL == this->m_advanced)
				{
					//bool lockTaken = false;
					try
					{
						Monitor::Enter(this->m_syncLock);
						//Monitor::Enter(this->m_syncLock, lockTaken);
						if(NULL == this->m_advanced)
						{
							DbgAdvanced* debugAdvanced = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgAdvanced),(void **)&debugAdvanced));
							this->m_advanced = debugAdvanced;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_advanced;
			}

			DbgClient *DebuggeeInfo::Client::get()
			{
				return this->m_client;
			}

			DbgControl *DebuggeeInfo::Control::get()
			{
				this->CheckDisposed();

				if(NULL == this->m_control)
				{
					//bool lockTaken = false;
					try
					{
						//Monitor::Enter(this->m_syncLock, lockTaken);
						Monitor::Enter(this->m_syncLock);
						if(NULL == this->m_control)
						{
							DbgControl* debugControl = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgControl),(void **)&debugControl));
							this->m_control = debugControl;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_control;
			}
			DbgDataSpaces *DebuggeeInfo::DataSpaces::get()
			{
				this->CheckDisposed();

				if(NULL == this->m_dataSpaces)
				{
					//bool lockTaken = false;
					try
					{
						//Monitor::Enter(this->m_syncLock, lockTaken);
						Monitor::Enter(this->m_syncLock);
						if(NULL == this->m_dataSpaces)
						{
							DbgDataSpaces* debugDataSpaces = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgDataSpaces),(void **)&debugDataSpaces));
							this->m_dataSpaces = debugDataSpaces;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_dataSpaces;
			}

			DbgRegisters *DebuggeeInfo::Registers::get()
			{
				this->CheckDisposed();

				if(NULL == this->m_registers)
				{
					//bool lockTaken = false;
					try
					{
						Monitor::Enter(this->m_syncLock);
						//Monitor::Enter(this->m_syncLock, lockTaken);
						if(NULL == this->m_registers)
						{
							DbgRegisters* registers = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgRegisters),(void **)&registers));
							this->m_registers = registers;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_registers;
			}

			DbgSymbols *DebuggeeInfo::Symbols::get()
			{
				this->CheckDisposed();

				if(NULL == this->m_symbols)
				{
					//bool lockTaken = false;
					try
					{
						//Monitor::Enter(this->m_syncLock, lockTaken);
						Monitor::Enter(this->m_syncLock);
						if(NULL == this->m_symbols)
						{
							DbgSymbols* debugSymbols = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgSymbols),(void **)&debugSymbols));
							this->m_symbols = debugSymbols;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_symbols;
			}

			DbgSystemObjects *DebuggeeInfo::SystemObjects::get()
			{
				this->CheckDisposed();

				if(NULL == this->m_systemObjects)
				{
					//bool lockTaken = false;
					try
					{
						//Monitor::Enter(this->m_syncLock, lockTaken);
						Monitor::Enter(this->m_syncLock);
						if(NULL == this->m_systemObjects)
						{
							DbgSystemObjects* debugSystemObjects = NULL;
							Tools::CheckHR(this->Client->QueryInterface(__uuidof(DbgSystemObjects),(void **)&debugSystemObjects));
							this->m_systemObjects = debugSystemObjects;
						}
					}
					finally
					{
						//if(lockTaken)
							Monitor::Exit(this->m_syncLock);
					}
				}

				return this->m_systemObjects;
			}

			MS::Debuggers::DbgEng::DebugAdvanced^ DebuggeeInfo::DebugAdvanced::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugAdvanced)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugAdvanced)
					{
						this->m_debugAdvanced = gcnew MS::Debuggers::DbgEng::DebugAdvanced(this);
					}
				}

				return this->m_debugAdvanced;
			}

			MS::Debuggers::DbgEng::DebugClient^ DebuggeeInfo::DebugClient::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugClient)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugClient)
					{
						this->m_debugClient = gcnew MS::Debuggers::DbgEng::DebugClient(this->Client);
					}
				}

				return this->m_debugClient;
			}
			MS::Debuggers::DbgEng::DebugControl^ DebuggeeInfo::DebugControl::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugControl)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugControl)
					{
						this->m_debugControl = gcnew MS::Debuggers::DbgEng::DebugControl(this);
					}
				}

				return this->m_debugControl;
			}
			MS::Debuggers::DbgEng::DebugDataSpaces^ DebuggeeInfo::DebugDataSpaces::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugDataSpaces)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugDataSpaces)
					{
						this->m_debugDataSpaces = gcnew MS::Debuggers::DbgEng::DebugDataSpaces(this->DataSpaces);
					}
				}

				return this->m_debugDataSpaces;
			}

			MS::Debuggers::DbgEng::DebugRegisters^ DebuggeeInfo::DebugRegisters::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugRegisters)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugRegisters)
					{
						this->m_debugRegisters = gcnew MS::Debuggers::DbgEng::DebugRegisters(this);
					}
				}

				return this->m_debugRegisters;
			}


			MS::Debuggers::DbgEng::DebugSymbols^ DebuggeeInfo::DebugSymbols::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugSymbols)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugSymbols)
					{
						this->m_debugSymbols = gcnew MS::Debuggers::DbgEng::DebugSymbols(this->Symbols);
					}
				}

				return this->m_debugSymbols;
			}
			MS::Debuggers::DbgEng::DebugSystemObjects^ DebuggeeInfo::DebugSystemObjects::get()
			{
				this->CheckDisposed();

				if(nullptr == this->m_debugSystemObjects)
				{
					msclr::lock l(this->m_syncLock);
					if(nullptr == this->m_debugSystemObjects)
					{
						this->m_debugSystemObjects = gcnew MS::Debuggers::DbgEng::DebugSystemObjects(this->SystemObjects);
					}
				}

				return this->m_debugSystemObjects;
			}
		}
	}
}
