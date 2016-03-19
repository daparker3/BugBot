#include "stdafx.h"
#include "DbgBreak.h"
#include "Debuggee.h"

using namespace System;
using namespace Runtime::InteropServices;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgBreak::DbgBreak(DbgBreakpoint* bp)
				: _bp(bp),
				_disposed(false)
			{
				_bp->AddRef();
			}
			DbgBreak::~DbgBreak()
			{
				this->!DbgBreak();
				_disposed = true;
			}
			DbgBreak::!DbgBreak()
			{
				if (_bp)
				{
					_bp->Release();
					_bp = nullptr;
				}
			}
			BreakpointFlags DbgBreak::GetFlags()
			{
				ULONG ret;
				Tools::CheckHR(_bp->GetFlags(&ret));
				return (BreakpointFlags)ret;
			}
			void DbgBreak::AddFlags(BreakpointFlags flags)
			{
				Tools::CheckHR(_bp->AddFlags((ULONG)flags));
			}
			void DbgBreak::RemoveFlags(BreakpointFlags flags)
			{
				Tools::CheckHR(_bp->RemoveFlags((ULONG)flags));
			}
			void DbgBreak::SetFlags(BreakpointFlags flags)
			{
				Tools::CheckHR(_bp->SetFlags((ULONG)flags));
			}
			String^ DbgBreak::GetCommand()
			{
				WCHAR buf[4096];
				ULONG strLen = 0;
				Tools::CheckHR(_bp->GetCommandWide(buf, ARRAYSIZE(buf), &strLen));
				return gcnew String(buf);
			}
			void DbgBreak::SetCommand(String^ command)
			{
				pin_ptr<const wchar_t> pCommand = PtrToStringChars(command);
				Tools::CheckHR(_bp->SetCommandWide(pCommand));
			}
			BreakpointDataParameters^ DbgBreak::GetDataParameters()
			{
				ULONG size;
				ULONG accessType;
				Tools::CheckHR(_bp->GetDataParameters(&size, &accessType));
				return gcnew BreakpointDataParameters(size, accessType);
			}
			void DbgBreak::SetDataParameters(BreakpointDataParameters^ parameters)
			{
				Tools::CheckHR(_bp->SetDataParameters(parameters->Size, parameters->AccessType));
			}
			ULONG DbgBreak::GetId()
			{
				ULONG id;
				Tools::CheckHR(_bp->GetId(&id));
				return id;
			}
			ULONG DbgBreak::GetMatchThreadId()
			{
				ULONG matchId;
				Tools::CheckHR(_bp->GetMatchThreadId(&matchId));
				return matchId;
			}
			void DbgBreak::SetMatchThreadId(ULONG thread)
			{
				Tools::CheckHR(_bp->SetMatchThreadId(thread));
			}
			ULONG64 DbgBreak::GetOffset()
			{
				ULONG64 offset;
				Tools::CheckHR(_bp->GetOffset(&offset));
				return offset;
			}
			void DbgBreak::SetOffset(ULONG64 offset)
			{
				Tools::CheckHR(_bp->SetOffset(offset));
			}
			String^ DbgBreak::GetOffsetExpression()
			{
				WCHAR offsetExpression[4096];
				ULONG strLen;
				Tools::CheckHR(_bp->GetOffsetExpressionWide(offsetExpression, ARRAYSIZE(offsetExpression), &strLen));
				return gcnew String(offsetExpression);
			}
			void DbgBreak::SetOffsetExpression(String^ expression)
			{
				pin_ptr<const wchar_t> pExpression = PtrToStringChars(expression);
				Tools::CheckHR(_bp->SetOffsetExpressionWide(pExpression));
			}
			ULONG DbgBreak::GetCurrentPassCount()
			{
				ULONG passCount;
				Tools::CheckHR(_bp->GetCurrentPassCount(&passCount));
				return passCount;
			}
			ULONG DbgBreak::GetPassCount()
			{
				ULONG passCount;
				Tools::CheckHR(_bp->GetPassCount(&passCount));
				return passCount;
			}
			void DbgBreak::SetPassCount(ULONG count)
			{
				Tools::CheckHR(_bp->SetPassCount(count));
			}
			BreakpointType^ DbgBreak::GetType()
			{
				ULONG breakType;
				ULONG procType;
				Tools::CheckHR(_bp->GetType(&breakType, &procType));
				return gcnew BreakpointType((BreakpointTypeFlags)breakType, procType);
			}
		}
	}
}
