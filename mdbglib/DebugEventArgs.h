#pragma once

#include "Stdafx.h"
#include "DbgBreak.h"
#include "NativeException.h"

using namespace System;

namespace MS
{
	namespace Debuggers
	{
		namespace DbgEng
		{
			public ref class DebuggerEventArgs : EventArgs
			{
			private:
				DebugStatus _status;
			public:
				DebuggerEventArgs()
				{
					_status = DebugStatus::NoChange;
				}
				property DebugStatus Status
				{
					DebugStatus get()
					{
						return _status;
					}
					void set(DebugStatus status)
					{
						_status = status;
					}
				}
			};
			public ref class BreakpointEventArgs : DebuggerEventArgs
			{
			private:
				DbgBreak^ _breakpoint;
			public:
				BreakpointEventArgs(DbgBreak^ breakpoint)
					: _breakpoint(breakpoint)
				{
				}
				property DbgBreak^ Breakpoint
				{
					DbgBreak^ get()
					{
						return _breakpoint;
					}
				}
			};
			public ref class CreateProcessEventArgs : DebuggerEventArgs
			{
			private:
				ULONG64 _imageFileHandle;
				ULONG64 _handle;
				ULONG64 _baseOffset;
				ULONG _moduleSize;
				String^ _moduleName;
				String^ _imageName;
				ULONG _checksum;
				ULONG _timeDateStamp;
				ULONG64 _initialThreadHandle;
				ULONG64 _threadDataOffset;
				ULONG64 _startOffset;
			public:
				CreateProcessEventArgs(
					ULONG64 imageFileHandle,
					ULONG64 handle,
					ULONG64 baseOffset,
					ULONG moduleSize,
					String^ moduleName,
					String^ imageName,
					ULONG checksum,
					ULONG timeDateStamp,
					ULONG64 initialThreadHandle,
					ULONG64 threadDataOffset,
					ULONG64 startOffset
				)
				: _imageFileHandle(imageFileHandle),
				_handle(handle),
				_baseOffset(baseOffset),
				_moduleSize(moduleSize),
				_moduleName(moduleName),
				_imageName(imageName),
				_checksum(checksum),
				_timeDateStamp(timeDateStamp),
				_initialThreadHandle(initialThreadHandle),
				_threadDataOffset(threadDataOffset),
				_startOffset(startOffset)
				{

				}

				property ULONG64 ImageFileHandle
				{
					ULONG64 get()
					{
						return _imageFileHandle;
					}
				}
				property ULONG64 Handle
				{
					ULONG64 get()
					{
						return _handle;
					}
				}
				property ULONG64 BaseOffset
				{
					ULONG64 get()
					{
						return _baseOffset;
					}
				}
				property ULONG ModuleSize
				{
					ULONG get()
					{
						return _moduleSize;
					}
				}
				property String^ ModuleName
				{
					String^ get()
					{
						return _moduleName;
					}
				}
				property String^ ImageName
				{
					String^ get()
					{
						return _imageName;
					}
				}
				property ULONG Checksum
				{
					ULONG get()
					{
						return _checksum;
					}
				}
				property ULONG TimeDateStamp
				{
					ULONG get()
					{
						return _timeDateStamp;
					}
				}
				property ULONG64 InitialThreadHandle
				{
					ULONG64 get()
					{
						return _initialThreadHandle;
					}
				}
				property ULONG64 ThreadDataOffset
				{
					ULONG64 get()
					{
						return _threadDataOffset;
					}
				}
				property ULONG64 StartOffset
				{
					ULONG64 get()
					{
						return _startOffset;
					}
				}
			};

			public ref class CreateThreadEventArgs : DebuggerEventArgs
			{
			private:
				ULONG64 _handle;
				ULONG64 _dataOffset;
				ULONG64 _startOffset;

			public:				
				CreateThreadEventArgs(
					ULONG64 handle,
					ULONG64 dataOffset,
					ULONG64 startOffset
				) : _handle(handle),
				_dataOffset(dataOffset),
				_startOffset(startOffset)
				{

				}
				property ULONG64 Handle
				{
					ULONG64 get()
					{
						return _handle;
					}
				}
				property ULONG64 DataOffset
				{
					ULONG64 get()
					{
						return _dataOffset;
					}
				}
				property ULONG64 StartOffset
				{
					ULONG64 get()
					{
						return _startOffset;
					}
				}
			};
			public ref class ExceptionEventArgs : DebuggerEventArgs
			{
			private:
				NativeException^ _exception;
				bool _firstChance;
			public:				
				ExceptionEventArgs(
					NativeException^ exception,
					bool firstChance
					) : _exception(exception),
					_firstChance(firstChance)
				{

				}
				property NativeException^ Exception
				{
					NativeException^ get()
					{
						return _exception;
					}
				}
				property bool FirstChance
				{
					bool get()
					{
						return _firstChance;
					}
				}
			};
			public ref class ExitProcessEventArgs : DebuggerEventArgs
			{
			private:
				ULONG _exitCode;
			public:
				ExitProcessEventArgs(ULONG exitCode)
					: _exitCode(exitCode)
				{
				}
				property ULONG ExitCode
				{
					ULONG get()
					{
						return _exitCode;
					}
				}
			};
			public ref class ExitThreadEventArgs : DebuggerEventArgs
			{
			private:
				ULONG _exitCode;
			public:
				ExitThreadEventArgs(ULONG exitCode)
					: _exitCode(exitCode)
				{
				}
				property ULONG ExitCode
				{
					ULONG get()
					{
						return _exitCode;
					}
				}
			};
			public ref class LoadModuleEventArgs : DebuggerEventArgs
			{
			private:
				ULONG64 _imageFileHandle;
				ULONG64 _baseOffset;
				ULONG _moduleSize;
				String^ _moduleName;
				String^ _imageName;
				ULONG _checkSum;
				ULONG _timeDateStamp;
			public:
				LoadModuleEventArgs(ULONG64 imageFileHandle,
					ULONG64 baseOffset,
					ULONG moduleSize,
					String^ moduleName,
					String^ imageName,
					ULONG checkSum,
					ULONG timeDateStamp
					)
					: _imageFileHandle(imageFileHandle),
					_baseOffset(baseOffset),
					_moduleSize(moduleSize),
					_moduleName(moduleName),
					_imageName(imageName),
					_checkSum(checkSum),
					_timeDateStamp(timeDateStamp)
				{
				}
				
				property ULONG64 ImageFileHandle
				{
					ULONG64 get()
					{
						return _imageFileHandle;
					}
				}
				property ULONG64 BaseOffset
				{
					ULONG64 get()
					{
						return _baseOffset;
					}
				}
				property ULONG ModuleSize
				{
					ULONG get()
					{
						return _moduleSize;
					}
				}
				property String^ ModuleName
				{
					String^ get()
					{
						return _moduleName;
					}
				}
				property String^ ImageName
				{
					String^ get()
					{
						return _imageName;
					}
				}
				property ULONG CheckSum
				{
					ULONG get()
					{
						return _checkSum;
					}
				}
				property ULONG TimeDateStamp
				{
					ULONG get()
					{
						return _timeDateStamp;
					}
				}
			};
			public ref class SessionStatusEventArgs : DebuggerEventArgs
			{
			private:
				SessionStatusFlags _flags;

			public:
				SessionStatusEventArgs(SessionStatusFlags flags)
					: _flags(flags)
				{
				}
				property SessionStatusFlags Flags
				{
					SessionStatusFlags get()
					{
						return _flags;
					}
				}
			};
			public ref class SystemErrorEventArgs : DebuggerEventArgs
			{
			private:
				ULONG _error;
				ULONG _level;

			public:
				SystemErrorEventArgs(ULONG error, ULONG level)
					: _error(error),
					_level(level)
				{
				}
				property ULONG Error
				{
					ULONG get()
					{
						return _error;
					}
				}
				property ULONG Level
				{
					ULONG get()
					{
						return _level;
					}
				}
			};
			public ref class UnloadModuleEventArgs : DebuggerEventArgs
			{
			private:
				String^ _imageBase;
				ULONG64 _baseOffset;

			public:
				UnloadModuleEventArgs(String^ imageBase, ULONG64 baseOffset)
					: _imageBase(imageBase),
					_baseOffset(baseOffset)
				{
				}
				property String^ ImageBase
				{
					String^ get()
					{
						return _imageBase;
					}
				}
				property ULONG64 BaseOffset
				{
					ULONG64 get()
					{
						return _baseOffset;
					}
				}
			};
		}
	}
}
