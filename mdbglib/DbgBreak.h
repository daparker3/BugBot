#pragma once
#include "Stdafx.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref struct BreakpointDataParameters
			{
			public:
				ULONG Size;
				ULONG AccessType;

				BreakpointDataParameters(ULONG size, ULONG accessType)
				{
					Size = size;
					AccessType = accessType;
				}
			};

			public ref struct BreakpointType
			{
			public:
				BreakpointTypeFlags BreakType;
				ULONG ProcType;

				BreakpointType(BreakpointTypeFlags breakType, ULONG procType)
				{
					BreakType = breakType;
					ProcType = procType;
				}
			};

			ref class Debuggee;

			public ref class DbgBreak
			{
			private:
				DbgBreakpoint* _bp;
				bool _disposed;
			internal:
				DbgBreak(DbgBreakpoint* bp);
				~DbgBreak();
				!DbgBreak();
			public:
				BreakpointFlags GetFlags();
				void AddFlags(BreakpointFlags flags);
				void RemoveFlags(BreakpointFlags flags);
				void SetFlags(BreakpointFlags flags);
				String^ GetCommand();
				void SetCommand(String^ command);
				BreakpointDataParameters^ GetDataParameters();
				void SetDataParameters(BreakpointDataParameters^ parameters);
				ULONG GetId();
				ULONG GetMatchThreadId();
				void SetMatchThreadId(ULONG thread);
				ULONG64 GetOffset();
				void SetOffset(ULONG64 offset);
				String^ GetOffsetExpression();
				void SetOffsetExpression(String^ expression);
				ULONG GetCurrentPassCount();
				ULONG GetPassCount();
				void SetPassCount(ULONG count);
				BreakpointType^ GetType();
			};
		}
	}
}