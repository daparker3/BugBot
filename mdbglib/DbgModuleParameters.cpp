#include "StdAfx.h"
#include "DbgModuleParameters.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgModuleParameters::DbgModuleParameters(_DEBUG_MODULE_PARAMETERS params)
			{
				this->m_base = params.Base;
				this->m_size = params.Size;
				this->m_timeDateStamp = params.TimeDateStamp;
				this->m_checksum = params.Checksum;
				this->m_flags = params.Flags;
				this->m_symbolType = (MS::Debuggers::DbgEng::SymbolMode)params.SymbolType;
				this->m_imageNameSize = params.ImageNameSize;
				this->m_moduleNameSize = params.ModuleNameSize;
				this->m_loadedImageNameSize = params.LoadedImageNameSize;
				this->m_symbolFileNameSize = params.SymbolFileNameSize;
				this->m_mappedImageNameSize = params.MappedImageNameSize;
				this->m_reserved = gcnew array<ULONG64>(2);
				this->m_reserved[0] = params.Reserved[0];
				this->m_reserved[1] = params.Reserved[1];
			}
		}
	}
}
