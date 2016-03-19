#pragma once

#define CMD_PORT 8002

#define DEBUG_STRING L"123easy"

enum DebugCommand
{
	Exit = 'x',
	Breakpoint = 'b',
	DebugString = 'd',
	FirstChanceException = 'f',
	SecondChanceException = 'e',

};

DWORD WINAPI PongThread(LPVOID lpParameter);
DWORD WINAPI CommandThread(LPVOID lpParameter);
void CommandStateMachine(char buf);

extern HANDLE ExitEvent;