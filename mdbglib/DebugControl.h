#pragma once

#include "DbgFrame.h"
#include "DbgValue.h"
#include "DbgEnums.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugControl
			{
			private:
				DebuggeeInfo ^m_info;
				bool m_disposed;
			internal:
				DebugControl(DebuggeeInfo^ info);
				!DebugControl();
				~DebugControl();

			public:
				property ULONG InterruptTimeout
				{
					ULONG get();
					void set(ULONG interruptTimeout);
				}

				bool GetIsInterruptRequested();
				void SetInterrupt(DebugInterruptFlags interruptFlags);
				array<DbgFrame^>^ GetStoredStackTrace(ULONG maxFrames);
				DbgValue^ Evaluate(String ^expression, DbgValueType desiredType, PULONG remainderIndex);
				DbgValue^ Evaluate(String ^expression, DbgValueType desiredType, PULONG remainderIndex, DbgExpressionType exprType);
				bool WaitForEvent(ULONG timeout);
				void GetStoredEventInformation([Out] DebugEvent %ev, [Out] ULONG %processId, [Out] ULONG %threadId, [Out] array<BYTE>^ %context, [Out] array<BYTE>^ %extraInformation);
				DebugStatus GetExecutionStatus();
				void Execute(DebugOutputControl control, String ^command, DebugExecuteFlags flags);
				property EngineOptionsEnum EngineOptions
				{
					EngineOptionsEnum get();
					void set(EngineOptionsEnum in_eOptions);
				}
			};
		}
	}
}