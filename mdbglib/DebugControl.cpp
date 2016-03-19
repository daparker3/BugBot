#include "StdAfx.h"
#include "DebugControl.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{

			DebugControl::DebugControl(DebuggeeInfo ^info)
			{
				this->m_info = info;
				this->m_disposed = false;
			}

			DebugControl::~DebugControl()
			{
				this->!DebugControl();
				this->m_disposed = true;
			}
			DebugControl::!DebugControl()
			{
			}

			ULONG DebugControl::InterruptTimeout::get()
			{
				ULONG timeout;
				Tools::CheckHR(this->m_info->Control->GetInterruptTimeout(&timeout));
				return timeout;
			}
			void DebugControl::InterruptTimeout::set(ULONG interruptTimeout)
			{
				Tools::CheckHR(this->m_info->Control->SetInterruptTimeout(interruptTimeout));
			}

			bool DebugControl::GetIsInterruptRequested()
			{
				HRESULT hr = this->m_info->Control->GetInterrupt();
				if (FAILED(hr))
				{
					Tools::CheckHR(hr);
				}
				return hr == S_OK;
			}
			void DebugControl::SetInterrupt(DebugInterruptFlags interruptFlags)
			{
				Tools::CheckHR(this->m_info->Control->SetInterrupt((ULONG)interruptFlags));
			}

			array<DbgFrame^>^ DebugControl::GetStoredStackTrace(ULONG maxFrames)
			{
				array<DbgFrame^>^ result = nullptr;
				PDEBUG_STACK_FRAME pStackFrames = NULL;
				PBYTE scopeContexts = NULL;
				PBYTE startContext = NULL;
				try
				{
					ULONG type, procId, thId;

					ULONG scopeContextSize = 0;
					//Get the size of stored context
					Tools::CheckHR(this->m_info->Control->GetStoredEventInformation(&type, &procId, &thId, NULL, 0, &scopeContextSize, NULL, 0, NULL));
					scopeContexts = new BYTE[scopeContextSize * maxFrames];
					startContext = new BYTE[scopeContextSize];

					//Tools::CheckHR(debuggeeInfo->Advanced->GetThreadContext(startContext, scopeContextSize));
					Tools::CheckHR(this->m_info->Symbols->GetScope(NULL, NULL, startContext, scopeContextSize));
	
					ULONG framesFilled;
					pStackFrames = new _DEBUG_STACK_FRAME[maxFrames];
					Tools::CheckHR(this->m_info->Control->GetContextStackTrace(startContext, scopeContextSize, pStackFrames, maxFrames, scopeContexts, scopeContextSize * maxFrames, scopeContextSize, &framesFilled));

					result = gcnew array<DbgFrame^>(framesFilled);
					for(ULONG i=0;i<framesFilled && i < maxFrames;i++)
					{
						array<BYTE>^ mScopeContext = gcnew array<BYTE>(scopeContextSize);
						for(ULONG j=0; j<scopeContextSize; j++)
							mScopeContext[j] = scopeContexts[i * scopeContextSize + j];

						result[i] = gcnew DbgFrame(this->m_info, pStackFrames[i], mScopeContext);
					}
				}
				finally
				{
					delete[] pStackFrames;
					delete[] scopeContexts;
					delete[] startContext;
				}
				return result;
			}

			DbgValue^ DebugControl::Evaluate(String ^expression, DbgValueType desiredType, PULONG remainderIndex, DbgExpressionType exprType)
			{
				this->m_info->Control->SetExpressionSyntax((ULONG)exprType);
				return DebugControl::Evaluate(expression, desiredType, remainderIndex);
			}
			DbgValue^ DebugControl::Evaluate(String ^expression, DbgValueType desiredType, PULONG remainderIndex)
			{
//HRESULT
//  DbgControl::EvaluateWide(
//    IN PCWSTR  Expression,
//    IN ULONG  DesiredType,
//    OUT PDEBUG_VALUE  Value,
//    OUT OPTIONAL PULONG  RemainderIndex
//    );
				pin_ptr<const wchar_t> pExpression = PtrToStringChars(expression);
				DEBUG_VALUE value;
				Tools::CheckHR(this->m_info->Control->EvaluateWide(pExpression, (ULONG)desiredType, &value, remainderIndex));
				return gcnew DbgValue(&value);
			}
			bool DebugControl::WaitForEvent(ULONG timeout)
			{
//HRESULT
//  IDebugControl::WaitForEvent(
//    IN ULONG  Flags,
//    IN ULONG  Timeout
//    );
				HRESULT hr = this->m_info->Control->WaitForEvent(0, timeout);
				return hr == S_OK;
			}
			void DebugControl::GetStoredEventInformation([Out] DebugEvent %ev, [Out] ULONG %processId, [Out] ULONG %threadId, [Out] array<BYTE>^ %context, [Out] array<BYTE>^ %extraInformation)
			{
//HRESULT
//  IDebugControl4::GetStoredEventInformation(
//    OUT PULONG  Type,
//    OUT PULONG  ProcessId,
//    OUT PULONG  ThreadId,
//    OUT OPTIONAL PVOID  Context,
//    IN ULONG  ContextSize,
//    OUT OPTIONAL PULONG  ContextUsed,
//    OUT OPTIONAL PVOID  ExtraInformation,
//    IN ULONG  ExtraInformationSize,
//    OUT OPTIONAL PULONG  ExtraInformationUsed)
				ULONG nType;
				ULONG nProcessId;
				ULONG nThreadId;
				ULONG nContextSize;
				ULONG nExtraInfoSize;
				Tools::CheckHR(this->m_info->Control->GetStoredEventInformation(&nType, &nProcessId, &nThreadId, NULL, 0, &nContextSize, NULL, 0, &nExtraInfoSize));
				context = gcnew array<BYTE>(nContextSize);
				extraInformation = gcnew array<BYTE>(nExtraInfoSize);
				pin_ptr<BYTE> pContext = &context[0];
				pin_ptr<BYTE> pExtraInfo = &extraInformation[0];
				Tools::CheckHR(this->m_info->Control->GetStoredEventInformation(&nType, &nProcessId, &nThreadId, pContext, nContextSize, NULL, pExtraInfo, nExtraInfoSize, NULL));

				ev = (DebugEvent)nType;
				processId = nProcessId;
				threadId = nThreadId;
			}
			DebugStatus DebugControl::GetExecutionStatus()
			{
				ULONG status;
				Tools::CheckHR(this->m_info->Control->GetExecutionStatus(&status));
				return (DebugStatus)status;
			}
			void DebugControl::Execute(DebugOutputControl control, String ^command, DebugExecuteFlags flags)
			{
//HRESULT
//  IDebugControl4::ExecuteWide(
//    IN ULONG  OutputControl,
//    IN PCWSTR  Command,
//    IN ULONG  Flags
//    );
				pin_ptr<const wchar_t> pCommand = PtrToStringChars(command);
				Tools::CheckHR(this->m_info->Control->ExecuteWide((ULONG)control, pCommand, (ULONG)flags));
			}

			EngineOptionsEnum DebugControl::EngineOptions::get()
			{
				ULONG options;
				Tools::CheckHR(this->m_info->Control->GetEngineOptions(&options));
				return (EngineOptionsEnum)options;
			}

			void DebugControl::EngineOptions::set(EngineOptionsEnum in_eOptions)
			{
				Tools::CheckHR(this->m_info->Control->SetEngineOptions((ULONG)in_eOptions));
			}
		}
	}
}
