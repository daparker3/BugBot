#include "StdAfx.h"
#include "DbgModule.h"

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			DbgModule::DbgModule(DebuggeeInfo ^info, _DEBUG_MODULE_PARAMETERS nparams, ULONG index) : m_params(nparams)
			{
				this->m_info = info;
				this->m_base = this->m_params.Base;
				this->m_index = index;
				this->m_isInited = true;
			}

			DbgModule::DbgModule(DebuggeeInfo ^info, ULONG64 base) : m_params()
			{
				this->m_info = info;
				this->m_base = base;
				this->m_index = DEBUG_ANY_ID;
				this->m_isInited = false;
			}

			DbgModule::DbgModule(DebuggeeInfo ^info, ULONG64 base, ULONG index) : m_params()
			{
				this->m_info = info;
				this->m_base = base;
				this->m_index = index;
				this->m_isInited = false;
			}

			void DbgModule::EnsureParams()
			{
				if(this->m_isInited)
					return;

				ULONG64 base = this->m_base;
				_DEBUG_MODULE_PARAMETERS nparams;
				Tools::CheckHR(this->m_info->Symbols->GetModuleParameters(1, &base, 0, &nparams));
				this->m_params = DbgModuleParameters(nparams);

				this->m_imageName		= nullptr;
				this->m_name			= nullptr;
				this->m_loadedImageName = nullptr;
				this->m_symbolFileName  = nullptr;
				this->m_mappedImageName = nullptr;

				this->m_isInited = true;
			}

			ULONG64 DbgModule::Base::get()
			{
				return this->m_base;
			}

			ULONG DbgModule::Size::get()
			{
				this->EnsureParams();
				return this->m_params.Size;
			}

			DateTime DbgModule::TimeStamp::get()
			{
				this->EnsureParams();
				return DateTime(this->m_params.TimeDateStamp);
			}

			ULONG DbgModule::Checksum::get()
			{
				this->EnsureParams();
				return this->m_params.Checksum;
			}

			ModuleFlags DbgModule::Flags::get()
			{
				this->EnsureParams();
				return (ModuleFlags)this->m_params.Flags;
			}

			SymbolMode DbgModule::SymbolType::get()
			{
				this->EnsureParams();
				return this->m_params.SymbolType;
			}

			String ^DbgModule::ImageName::get()
			{
				this->EnsureParams();

				if(nullptr == this->m_imageName)
					this->m_imageName = this->m_info->DebugSymbols->GetModuleNameString(ModuleNameType::Image, this->m_index, this->m_base, this->m_params.m_imageNameSize);

				return this->m_imageName;
			}

			String ^DbgModule::Name::get()
			{
				this->EnsureParams();
				
				if(nullptr == this->m_name)
					this->m_name = this->m_info->DebugSymbols->GetModuleNameString(ModuleNameType::Module, this->m_index, this->m_base, this->m_params.m_moduleNameSize);

				return this->m_name;
			}

			String ^DbgModule::LoadedImageName::get()
			{
				this->EnsureParams();

				if(nullptr == this->m_loadedImageName)
					this->m_loadedImageName = this->m_info->DebugSymbols->GetModuleNameString(ModuleNameType::LoadedImage, this->m_index, this->m_base, this->m_params.m_loadedImageNameSize);

				return this->m_loadedImageName;
			}

			String ^DbgModule::SymbolFileName::get()
			{
				this->EnsureParams();

				if(nullptr == this->m_symbolFileName)
					this->m_symbolFileName = this->m_info->DebugSymbols->GetModuleNameString(ModuleNameType::SymbolFile, this->m_index, this->m_base, this->m_params.m_symbolFileNameSize);

				return this->m_symbolFileName;
			}

			String ^DbgModule::MappedImageName::get()
			{
				this->EnsureParams();

				if(nullptr == this->m_mappedImageName)
					this->m_mappedImageName = this->m_info->DebugSymbols->GetModuleNameString(ModuleNameType::MappedImage, this->m_index, this->m_base, this->m_params.m_mappedImageNameSize);

				return this->m_mappedImageName;
			}
#pragma region Types
			DbgType^ DbgModule::GetDbgType(String ^typeName)
			{
				ULONG typeId = this->m_info->DebugSymbols->GetTypeId(this->m_base, typeName);
				return gcnew DbgType(this->m_info, this->m_base, typeId);
			}
#pragma endregion

			//DbgObject^ DbgModule::Global::get()
			//{
			//	//DebuggeeInfo^ info, ULONG64 offset, ULONG64 moduleBase, ULONG typeId
			//	return gcnew DbgObject(this->m_info, 0, this->m_base, 0);
			//}

		}
	}
}
