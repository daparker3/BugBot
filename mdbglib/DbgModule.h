#pragma once

#include "DbgModuleParameters.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DbgModule
			{
			private: 
				ULONG64 m_base;
				ULONG m_index;
				DebuggeeInfo^ m_info;
				bool m_isInited;
				DbgModuleParameters m_params;

				String ^m_imageName;
				String ^m_name;
				String ^m_loadedImageName;
				String ^m_symbolFileName;
				String ^m_mappedImageName;

				void EnsureParams();
			internal:
				DbgModule(DebuggeeInfo ^info, ULONG64 base);
				DbgModule(DebuggeeInfo ^info, ULONG64 base, ULONG index);
				DbgModule(DebuggeeInfo ^info, _DEBUG_MODULE_PARAMETERS nparams, ULONG index);
			public:

				property ULONG64 Base { ULONG64 get(); }
				property ULONG Size{ ULONG get(); }
				property DateTime TimeStamp { DateTime get(); }
				property ULONG Checksum { ULONG get(); }
				property ModuleFlags Flags { ModuleFlags get(); }
				property SymbolMode SymbolType { SymbolMode get(); }

				property String ^ImageName { String ^get(); }
				property String ^Name { String ^get(); }
				property String ^LoadedImageName { String ^get(); }
				property String ^SymbolFileName { String ^get(); }
				property String ^MappedImageName { String ^get(); }

#pragma region Types
				DbgType^ GetDbgType(String ^typeName);
#pragma endregion

				//property DbgObject^ Global { DbgObject^ get(); }

			};
		}
	}
}