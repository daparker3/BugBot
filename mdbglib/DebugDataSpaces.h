#pragma once

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebugDataSpaces
			{
			private:
				DbgDataSpaces *m_dataSpaces;
				bool m_disposed;
			internal:
				DebugDataSpaces(DbgDataSpaces *);
				!DebugDataSpaces();
				~DebugDataSpaces();

			public:
				String^ ReadUnicodeStringVirtual(ULONG64 offset, ULONG maxChars);
				String^ ReadTruncatedUnicodeStringVirtual(ULONG64 offset, ULONG maxChars);
				String^ ReadTruncatedAnsiStringVirtual(ULONG64 offset, ULONG maxChars);
			};
		}
	}
}
