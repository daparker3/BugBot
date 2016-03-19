using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using EnsureThat;

namespace MS.BugBot.Service
{
    using Interfaces;
    using Debuggers.DbgEng;

    class DebugMonitor : IDebugMonitor, IDisposable
    {
        const int PollTime = 1000;

        string _connStr;
        string[] _symbols;
        AutoResetEvent _debuggerUpdatedEvent = new AutoResetEvent(false);
        Task _debugTask = null;
        Task _monitorTask = null;
        Debuggee _debuggee;
        SessionStatusFlags _sessionStatus = SessionStatusFlags.SessionEnd;
        object _outputListenerLock = new object();
        List<IDebugOutputListener> _outputListeners = new List<IDebugOutputListener>();

        object _triggerListenerLock = new object();
        List<IDebugEventListener> _triggerListeners = new List<IDebugEventListener>();
        bool _initialized = false;
        bool _debugKernel = false;
        bool _disposed = false;
        object _statusLock = new object();
        LastDebuggerStatus _status = new LastDebuggerStatus
        {
            ExitReason = String.Empty,
            State = DebuggerState.NotStarted,
            Fatal = false
        };

        public event EventHandler ShouldDispose;

        public void ConnectToProcessServer(string connStr, bool debugKernel)
        {
            if (_initialized)
            {
                throw new DebugMonitorException("Connection attempt was already made.");
            }

            _initialized = true;
            _debugKernel = debugKernel;
            _connStr = connStr;
            try
            {
                CreateDebuggee();
                _debugTask = Task.Factory.StartNew(DebuggerTask, TaskCreationOptions.LongRunning);
                _monitorTask = Task.Factory.StartNew(DebuggerMonitor, TaskCreationOptions.LongRunning);
            }
            catch (COMException ce)
            {
                throw new DebugMonitorException("Failed to connect to process server.", ce);
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Failed to connect to process server.", ic);
            }
        }

        public void SetSymbols(string[] symbols)
        {
            _symbols = symbols;
        }

        public void EndSession(bool force)
        {
            if (force)
            {
                lock (_statusLock)
                {
                    if (_status.State == DebuggerState.Running)
                    {
                        TriggerExit("The client ended the session.");
                        _debuggee.DebugControl.SetInterrupt(DebugInterruptFlags.Active);
                    }
                }
            }

            try
            {
                Task.WaitAny(new Task[] { _debugTask, _monitorTask });
            }
            finally
            {
                if (ShouldDispose != null)
                {
                    ShouldDispose(this, new EventArgs());
                }
            }
        }

        public ProcessInfo[] GetRunningProcesses()
        {
            if (_debugKernel)
            {
                throw new InvalidOperationException("The monitor is debugging a kernel.");
            }

            try
            {
                List<ProcessInfo> ret = new List<ProcessInfo>();

                foreach (uint processId in _debuggee.Processes)
                {
                    string procName = _debuggee.GetProcessName(processId, GetRunningProcessDescriptionFlags.NoPaths);

                    if (processId != 0 && !String.IsNullOrEmpty(procName))
                    {
                        ProcessInfo procInfo = new ProcessInfo
                        {
                            ImageName = procName,
                            ProcessId = (int)processId,
                        };

                        ret.Add(procInfo);
                    }
                }

                return ret.ToArray();
            }
            catch (COMException ce)
            {
                throw new DebugMonitorException("Failed to get process list.", ce);
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Failed to get process list.", ic);
            }
        }

        public void CreateAndAttachToProcess(string commandLine, string initialDirectory)
        {
            if (_debugKernel)
            {
                throw new DebugMonitorException("The monitor is debugging a kernel.");
            }

            try
            {
                _debuggee.CreateAndAttachProcess(commandLine, initialDirectory);
                _debuggerUpdatedEvent.Set();
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Create failed.", ic);
            }
        }

        public void AttachToProcessByName(string imageName)
        {
            if (_debugKernel)
            {
                throw new DebugMonitorException("The monitor is debugging a kernel.");
            }

            try
            {
                uint procId = _debuggee.GetSystemId(imageName, GetRunningProcessSystemIdByExecutableNameFlags.ProcOnlyMatch);
                _debuggee.AttachProcess(procId, AttachFlags.InvasiveNoInitialBreak | AttachFlags.InvasiveResumeProcess);
                _debuggerUpdatedEvent.Set();
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Attach failed.", ic);
            }
        }

        public void AttachToProcessById(int processId)
        {
            if (_debugKernel)
            {
                throw new DebugMonitorException("The monitor is debugging a kernel.");
            }

            try
            {
                _debuggee.AttachProcess((uint)processId, AttachFlags.InvasiveNoInitialBreak | AttachFlags.InvasiveResumeProcess);
                _debuggerUpdatedEvent.Set();
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Attach failed.", ic);
            }
        }

        public void AddOutputListener(IDebugOutputListener listener)
        {
            Ensure.That(listener, "listener").IsNotNull();

            lock (_outputListenerLock)
            {
                _outputListeners.Add(listener);
            }
        }

        public void AddTriggerListener(IDebugEventListener listener)
        {
            Ensure.That(listener, "listener").IsNotNull();

            lock (_triggerListenerLock)
            {
                _triggerListeners.Add(listener);
            }
        }

        public LastDebuggerStatus GetDebuggerStatus()
        {
            lock (_statusLock)
            {
                return _status;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    if (_debuggee != null)
                    {
                        _debuggee.Dispose();
                        _debuggee = null;
                    }

                    if (_debuggerUpdatedEvent != null)
                    {
                        _debuggerUpdatedEvent.Dispose();
                        _debuggerUpdatedEvent = null;
                    }
                }

                _disposed = true;
            }
        }

        ~DebugMonitor()
        {
            Dispose(false);
        }

        void DebuggerMonitor()
        {
            int updateInterval = (int)Properties.Settings.Default.MaxUpdateInterval.TotalMilliseconds;

            try
            {
                for (; ;)
                {
                    if (!_debuggerUpdatedEvent.WaitOne(updateInterval))
                    {
                        if (!_debugTask.Wait(0))
                        {
                            // The debugger might be dead. :(
                            TriggerFatalExit("The debug monitor became unresponsive and was timed out.");
                            _debuggee.DebugControl.SetInterrupt(DebugInterruptFlags.Exit);
                        }
                        break;
                    }
                }
            }
            catch (ObjectDisposedException)
            {
                // Either the debugger event of the debugger thread was disposed.
                // In either case, there's no risk of a timeout, the session ended anyway.
            }
        }

        Debuggee ConnectToDebuggee(string connectionString, string[] symbolPath)
        {
            Debuggee debuggee = null;
            if (_debugKernel)
            {
                debuggee = Debuggee.ConnectToKernel(connectionString);
            }
            else
            {
                debuggee = Debuggee.ConnectToProcessServer(connectionString);
            }

            foreach (string symFolder in symbolPath)
            {
                try
                {
                    debuggee.SymbolPathAppend(symFolder);
                }
                catch (COMException)
                {
                }
            }

            return debuggee;
        }

        void DebuggerTask()
        {
            try
            {
                Util.LogMemoryUsageStatistics("Inside debugger task thread");

                lock (_statusLock)
                {
                    _status.State = DebuggerState.NotStarted;
                    _status.ExitReason = "The session terminated before it could attach to the debuggee.";
                    _status.Fatal = true;
                }

                lock (_statusLock)
                {
                    _status.State = DebuggerState.Running;
                    _status.Fatal = false;
                }

                Stopwatch sw = new Stopwatch();
                sw.Start();

                DebuggerStatus lastStatus = DebuggerStatus.TimedOut;
#if DEBUG
                int numTicks = 0;
#endif

                while (_status.State == DebuggerState.Running)
                {
                    DebuggerStatus dbgStatus = _debuggee.Update();
                    long now = sw.ElapsedMilliseconds;

#if DEBUG

                    if (numTicks % 100 == 0)
                    {
                        Util.LogMemoryUsageStatistics("Tick" + numTicks);
                    }
                    ++numTicks;
#endif

                    if (_status.State != DebuggerState.ExitPending)
                    {
                        if (dbgStatus != DebuggerStatus.Ok && dbgStatus != DebuggerStatus.TimedOut)
                        {
                            if (_sessionStatus == SessionStatusFlags.SessionActive)
                            {
                                TriggerExit("The debug session terminated due to a debuggee system shutdown.");
                            }
                            else
                            {
                                // If we're waiting for an attach, we don't want to bang the COM+ server with RPC traffic.
                                // Add a bit of stabilization time.
                                _debuggerUpdatedEvent.WaitOne(PollTime);
                            }
                        }
                    }

                    lastStatus = dbgStatus;
                    _debuggerUpdatedEvent.Set();
                }

                if (!_debugKernel)
                {
                    _debuggee.DetachProcesses();
                }
            }
            catch (Exception ex)
            {
                lock (_statusLock)
                {
                    _status.ExitReason = "The debugger encountered a fatal error: " + ex.ToString();
                    _status.State = DebuggerState.Exited;
                }

                throw;
            }
            finally
            {
                lock (_statusLock)
                {
                    _status.State = DebuggerState.Exited;
                }
            }

            Util.LogMemoryUsageStatistics("Debugger task complete");

            if (_status.Fatal)
            {
                throw new DebugMonitorException(_status.ExitReason);
            }
        }

        void CreateDebuggee()
        {
            // Set various termination conditions for the debugger. 
            _debuggee = ConnectToDebuggee(_connStr, _symbols);
            _debuggee.DebugControl.EngineOptions = EngineOptionsEnum.NoExecuteRepeat | EngineOptionsEnum.AllowNetworkPaths;
            _debuggee.DebugControl.InterruptTimeout = (uint)Properties.Settings.Default.MaxUpdateInterval.TotalMilliseconds;
            Util.LogMemoryUsageStatistics("Connected to debuggee");
            _debuggee.SessionStatus += _debuggee_SessionStatus;
            _debuggee.DebugOutput += _debuggee_DebugOutput;
            _debuggee.Breakpoint += _debuggee_Breakpoint;
            _debuggee.Exception += _debuggee_Exception;
            _debuggee.ModuleLoaded += _debuggee_ModuleLoaded;
            _debuggee.ModuleUnloaded += _debuggee_ModuleUnloaded;
            _debuggee.PreBreakpoint += _debuggee_PreBreakpoint;
            _debuggee.PreException += _debuggee_PreException;
            _debuggee.PreModuleLoaded += _debuggee_PreModuleLoaded;
            _debuggee.PreModuleUnloaded += _debuggee_PreModuleUnloaded;
            _debuggee.PreProcessCreated += _debuggee_PreProcessCreated;
            _debuggee.PreProcessExited += _debuggee_PreProcessExited;
            _debuggee.PreSystemError += _debuggee_PreSystemError;
            _debuggee.PreThreadCreated += _debuggee_PreThreadCreated;
            _debuggee.PreThreadExited += _debuggee_PreThreadExited;
            _debuggee.ProcessCreated += _debuggee_ProcessCreated;
            _debuggee.ProcessExited += _debuggee_ProcessExited;
            _debuggee.SystemError += _debuggee_SystemError;
            _debuggee.ThreadCreated += _debuggee_ThreadCreated;
            _debuggee.ThreadExited += _debuggee_ThreadExited;
        }

        private void _debuggee_PreThreadCreated(object sender, CreateThreadEventArgs e)
        {
            BreakIfHandled(DebugTrigger.CreateThread, e);
        }

        private void _debuggee_ThreadCreated(object sender, CreateThreadEventArgs e)
        {
            HandleIfBroken(DebugTrigger.CreateThread, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreThreadExited(object sender, ExitThreadEventArgs e)
        {
            BreakIfHandled(DebugTrigger.ExitThread, e);
        }

        private void _debuggee_ThreadExited(object sender, ExitThreadEventArgs e)
        {
            HandleIfBroken(DebugTrigger.ExitThread, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreSystemError(object sender, SystemErrorEventArgs e)
        {
            BreakIfHandled(DebugTrigger.SystemError, e);
        }

        private void _debuggee_SystemError(object sender, SystemErrorEventArgs e)
        {
            HandleIfBroken(DebugTrigger.SystemError, e);
            e.Status = DebugStatus.GoNotHandled;
        }

        private void _debuggee_PreProcessCreated(object sender, CreateProcessEventArgs e)
        {
            BreakIfHandled(DebugTrigger.CreateProcess, e);
        }

        private void _debuggee_ProcessCreated(object sender, CreateProcessEventArgs e)
        {
            HandleIfBroken(DebugTrigger.CreateProcess, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreProcessExited(object sender, ExitProcessEventArgs e)
        {
            BreakIfHandled(DebugTrigger.ExitProcess, e);
        }

        private void _debuggee_ProcessExited(object sender, ExitProcessEventArgs e)
        {
            HandleIfBroken(DebugTrigger.ExitProcess, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreModuleUnloaded(object sender, UnloadModuleEventArgs e)
        {
            BreakIfHandled(DebugTrigger.UnloadModule, e);
        }

        private void _debuggee_ModuleUnloaded(object sender, UnloadModuleEventArgs e)
        {
            HandleIfBroken(DebugTrigger.UnloadModule, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreModuleLoaded(object sender, LoadModuleEventArgs e)
        {
            BreakIfHandled(DebugTrigger.LoadModule, e);
        }

        private void _debuggee_ModuleLoaded(object sender, LoadModuleEventArgs e)
        {
            HandleIfBroken(DebugTrigger.LoadModule, e);
            e.Status = DebugStatus.Go;
        }

        private void _debuggee_PreException(object sender, ExceptionEventArgs e)
        {
            BreakIfHandled(DebugTrigger.Exception, e);
        }

        private void _debuggee_Exception(object sender, ExceptionEventArgs e)
        {
            HandleIfBroken(DebugTrigger.Exception, e);
            e.Status = DebugStatus.GoNotHandled;
        }

        private void _debuggee_PreBreakpoint(object sender, BreakpointEventArgs e)
        {
            BreakIfHandled(DebugTrigger.Breakpoint, e);
        }
        private void _debuggee_Breakpoint(object sender, BreakpointEventArgs e)
        {
            HandleIfBroken(DebugTrigger.Breakpoint, e);
            e.Status = DebugStatus.Go;
        }

        private void BreakIfHandled(DebugTrigger trigger, DebuggerEventArgs a)
        {
            lock (_triggerListenerLock)
            {
                foreach (IDebugEventListener triggerListener in _triggerListeners)
                {
                    if (triggerListener.HandlesEvent(trigger, a))
                    {
                        a.Status = DebugStatus.Break;
                        return;
                    }
                }
            }
        }

        private void HandleIfBroken(DebugTrigger trigger, DebuggerEventArgs e)
        {
            lock (_triggerListenerLock)
            {
                foreach (IDebugEventListener triggerListener in _triggerListeners)
                {
                    if (triggerListener.HandlesEvent(trigger, e))
                    {
                        triggerListener.HandleEvent(trigger, e, new DebugMonitorContext(_debuggee));
                    }
                }
            }
        }

        private void _debuggee_DebugOutput(object sender, DebugOutputEventArgs e)
        {
            lock (_outputListenerLock)
            {
                DateTime now = DateTime.Now;

                foreach (IDebugOutputListener outListener in _outputListeners)
                {
                    outListener.Output(now, e.OutputFlags, e.Output);
                }
            }
        }

        void _debuggee_SessionStatus(object sender, SessionStatusEventArgs e)
        {
            if (_sessionStatus != e.Flags)
            {
                if (_sessionStatus == SessionStatusFlags.SessionActive)
                {
                    lock (_statusLock)
                    {
                        if (e.Flags != SessionStatusFlags.SessionActive)
                        {
                            TriggerExit("The debugger was detached from the debuggee.");
                        }
                    }
                }

                _sessionStatus = e.Flags;
            }
        }

        void TriggerExit(string exitReason)
        {
            lock (_statusLock)
            {
                if (_status.State != DebuggerState.ExitPending)
                {
                    _status.ExitReason = exitReason;
                    _status.State = DebuggerState.ExitPending;
                    _debuggerUpdatedEvent.Set();
                }
            }
        }

        void TriggerFatalExit(string exitReason)
        {
            lock (_statusLock)
            {
                if (_status.State != DebuggerState.ExitPending)
                {
                    _status.Fatal = true;
                    TriggerExit(exitReason);
                }
            }
        }
    }
}
