#pragma once

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public value class DbgModuleParameters
			{
				/*
    ULONG64 Base;
    ULONG Size;
    ULONG TimeDateStamp;
    ULONG Checksum;
    ULONG Flags;
    ULONG SymbolType;
    ULONG ImageNameSize;
    ULONG ModuleNameSize;
    ULONG LoadedImageNameSize;
    ULONG SymbolFileNameSize;
    ULONG MappedImageNameSize;
    ULONG64 Reserved[2];
				*/
			internal :
				ULONG64 m_base;
				ULONG m_size;
				ULONG m_timeDateStamp;
				ULONG m_checksum;
				ULONG m_flags;
				MS::Debuggers::DbgEng::SymbolMode m_symbolType;
				ULONG m_imageNameSize;
				ULONG m_moduleNameSize;
				ULONG m_loadedImageNameSize;
				ULONG m_symbolFileNameSize;
				ULONG m_mappedImageNameSize;
				array<ULONG64>^ m_reserved;

			internal:
				DbgModuleParameters(_DEBUG_MODULE_PARAMETERS params);

			public:
				property SymbolMode SymbolType
				{
					SymbolMode get()
					{
						return this->m_symbolType;
					}
				}
				property ULONG64 Base
				{
					ULONG64 get()
					{
						return this->m_base;
					}
				}
				property ULONG Size
				{
					ULONG get()
					{
						return this->m_size;
					}
				}
				property ULONG TimeDateStamp
				{
					ULONG get()
					{
						return this->m_timeDateStamp;
					}
				}
				property ULONG Checksum
				{
					ULONG get()
					{
						return this->m_checksum;
					}
				}
				property ULONG Flags
				{
					ULONG get()
					{
						return this->m_flags;
					}
				}
			};
		}
	}
}