// DebugTarget.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DebugTarget.h"

HANDLE ExitEvent = NULL;
HANDLE PongHandle = NULL;
HANDLE CommandHandle = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	BOOL Sockets = FALSE;

	wprintf_s(L"Starting debug target\n");

	ExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ExitEvent)
	{
		WSADATA Data = { 0 };
		if (WSAStartup(2, &Data) == 0)
		{
			Sockets = TRUE;
			PongHandle = CreateThread(NULL, 0, PongThread, NULL, 0, NULL);
			CommandHandle = CreateThread(NULL, 0, CommandThread, NULL, 0, NULL);
		}
	}

	HANDLE Handles[] = { ExitEvent, GetStdHandle(STD_INPUT_HANDLE)};

	for (;;)
	{
		int rc = WaitForMultipleObjects(ARRAYSIZE(Handles), Handles, FALSE, INFINITE);
		if (rc == WAIT_OBJECT_0)
		{
			break;
		}
		else if (rc == WAIT_OBJECT_0 + 1)
		{
			if (_kbhit())
			{
				int ch = _getch();
				CommandStateMachine(ch);
			}
		}
	}

	if (PongHandle)
	{
		CloseHandle(PongHandle);
	}
	if (CommandHandle)
	{
		CloseHandle(CommandHandle);
	}

	if (Sockets)
	{
		WSACleanup();
	}

	return 0;
}

