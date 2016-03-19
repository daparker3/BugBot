// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <winsdkver.h>

#include <dbgeng.h>
#include <dbghelp.h>
#include <vcclr.h>
#include <tchar.h>
#include <strsafe.h>
#include "DbgEnums.h"
#include "PCWSTRWrapper.h"
#include "Tools.h"
#include "DebuggeeInfo.h"
#include "DebugSystemObjects.h"
#include "DebugDataSpaces.h"
#include "DebugSymbolGroup.h"
#include "DebugControl.h"
#include "DebugAdvanced.h"
#include "DebugRegisters.h"
#include "DebugSymbols.h"
#include "DebugClient.h"
#include "DebuggeeEventWrappers.h"
