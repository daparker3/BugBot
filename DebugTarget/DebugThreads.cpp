
#include "stdafx.h"
#include "DebugTarget.h"

struct ClientState
{
	sockaddr clisa;
	int salen;
	SOCKET s;
};

DWORD WINAPI ClientThread(LPVOID lpParameter);

DWORD WINAPI PongThread(LPVOID lpParameter)
{
	for (;;)
	{
		if (WaitForSingleObject(ExitEvent, 5000) == WAIT_OBJECT_0)
		{
			break;
		}

		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s != INVALID_SOCKET)
		{
			int so_broadcast = TRUE;

			if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&so_broadcast, sizeof(so_broadcast)) != INVALID_SOCKET)
			{
				sockaddr_in sa = { 0 };
				BYTE pad[8] = { 0 };
				int addrLength = sizeof(sa);
				sa.sin_addr.S_un.S_addr = htonl(0xFFFFFFFF);
				sa.sin_family = AF_INET;
				sa.sin_port = htons(CMD_PORT);
				sendto(s, (const char*)pad, sizeof(pad), 0, (LPSOCKADDR)&sa, addrLength);
				closesocket(s);
			}
		}
	}

	return 0;
}

DWORD WINAPI CommandThread(LPVOID lpParameter)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s != INVALID_SOCKET)
	{
		WSAEVENT event = WSACreateEvent();
		if (event != WSA_INVALID_EVENT)
		{
			sockaddr_in sa = { 0 };
			sa.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			sa.sin_family = AF_INET;
			sa.sin_port = htons(CMD_PORT);

			if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) != SOCKET_ERROR)
			{
				if (listen(s, SOMAXCONN) != SOCKET_ERROR)
				{
					if (WSAEventSelect(s, event, FD_ACCEPT) != SOCKET_ERROR)
					{
						for (;;)
						{
							HANDLE WaitEvents[] = { ExitEvent, event };
							DWORD waited = WaitForMultipleObjects(ARRAYSIZE(WaitEvents), WaitEvents, FALSE, INFINITE);

							if (waited == WAIT_OBJECT_0)
							{
								break;
							}
							else if (waited == WAIT_OBJECT_0 + 1)
							{
								WSAResetEvent(event);

								sockaddr clisa;
								int clilen = sizeof(clisa);
								SOCKET conn = accept(s, &clisa, &clilen);
								if (conn != INVALID_SOCKET)
								{
									ClientState* cs = new ClientState();
									cs->clisa = clisa;
									cs->salen = clilen;
									cs->s = conn;
									
									HANDLE ConnThread = CreateThread(NULL, 0, ClientThread, (LPVOID)cs, 0, NULL);
									if (ConnThread)
									{
										CloseHandle(ConnThread);
									}
								}
							}
						}
					}
				}
			}

			WSACloseEvent(event);
		}
		closesocket(s);
	}

	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	ClientState* cs = (ClientState*)lpParameter;

	WSAEVENT RecvEvent = WSACreateEvent();
	if (RecvEvent != WSA_INVALID_EVENT)
	{
		if (WSAEventSelect(cs->s, RecvEvent, FD_READ | FD_CLOSE) != SOCKET_ERROR)
		{
			for (;;)
			{
				HANDLE ReadEvents[] = { ExitEvent, RecvEvent };
				DWORD waited = WaitForMultipleObjects(ARRAYSIZE(ReadEvents), ReadEvents, FALSE, INFINITE);
				if (waited == WAIT_OBJECT_0)
				{
					break;
				}
				else if (waited == WAIT_OBJECT_0 + 1)
				{
					WSAResetEvent(RecvEvent);

					char buf;
					int rc = recvfrom(cs->s, &buf, sizeof(buf), 0, &cs->clisa, &cs->salen);
					if (rc == SOCKET_ERROR)
					{
						int sockErr = WSAGetLastError();
						if (sockErr == WSAECONNABORTED || sockErr == WSAECONNREFUSED || sockErr == WSAECONNRESET)
						{
							break;
						}
					}
					CommandStateMachine(buf);
				}
			}
		}
		WSACloseEvent(RecvEvent);
	}
	closesocket(cs->s);
	delete cs;
	return 0;
}

void CommandStateMachine(char buf)
{
	switch (buf)
	{
	case DebugCommand::Breakpoint:
		wprintf_s(L"Debug break\n");
		DebugBreak();
		break;
	case DebugCommand::Exit:
		wprintf_s(L"Exiting\n");
		SetEvent(ExitEvent);
		break;
	case DebugCommand::FirstChanceException:
		wprintf_s(L"First chance exception\n");
		try
		{
			throw 1;
		}
		catch (int&)
		{

		}
		break;
	case DebugCommand::SecondChanceException:
		wprintf_s(L"Second chance exception\n");
		throw 1;
		break;
	case DebugCommand::DebugString:
		wprintf_s(L"Debug string\n");
		OutputDebugString(L"1");
		OutputDebugString(L"2");
		OutputDebugString(L"3");
		OutputDebugString(L"easy\n");
		break;
	}
}