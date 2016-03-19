using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

using MS.BugBot.Service;

namespace MS.BugBot.Interfaces
{
    /// <summary>
    /// Get the state of the debugger. This is what the state machine is currently doing.
    /// </summary>
    public enum DebuggerState
    {
        /// <summary>
        /// The debugger isn't started yet.
        /// </summary>
        NotStarted,

        /// <summary>
        /// The debugger is running.
        /// </summary>
        Running,

        /// <summary>
        /// The debugger session is terminating.
        /// </summary>
        ExitPending,

        /// <summary>
        /// The debugger has exited.
        /// </summary>
        Exited
    }

    /// <summary>
    /// Get the current status of the debugger. Primarily intended for instrumentation.
    /// </summary>
    public class LastDebuggerStatus
    {
        /// <summary>
        /// Get the last state of the debugger.
        /// </summary>
        public DebuggerState State { get; set; }

        /// <summary>
        /// Get the debugger exit reason, if it's not running anymore.
        /// </summary>
        public string ExitReason { get; set; }

        /// <summary>
        /// Get if the debugger encountered a fatal error upon exit.
        /// </summary>
        public bool Fatal { get; set; }
    }

    /// <summary>
    /// Represent running process info.
    /// </summary>
    public class ProcessInfo
    {
        /// <summary>
        /// Represent the image name of the process.
        /// </summary>
        public string ImageName { get; set; }

        /// <summary>
        /// Represent the ID of the process.
        /// </summary>
        public int ProcessId { get; set; }
    }

    /// <summary>
    /// Represent a debug script trigger.
    /// </summary>
    public enum DebugTrigger
    {
        /// <summary>
        /// If we have broken into a piece of code.
        /// </summary>
        Breakpoint,
        /// <summary>
        /// If a process or child process is being created.
        /// </summary>
        CreateProcess,
        /// <summary>
        /// If a thread is being created.
        /// </summary>
        CreateThread,
        /// <summary>
        /// If a first or second-chance exception is being generated.
        /// </summary>
        Exception,
        /// <summary>
        /// If a thread is being exited.
        /// </summary>
        ExitThread,
        /// <summary>
        /// If a process is being exited.
        /// </summary>
        ExitProcess,
        /// <summary>
        /// If a module is being loaded.
        /// </summary>
        LoadModule,
        /// <summary>
        /// If a module is being unloaded.
        /// </summary>
        UnloadModule,
        /// <summary>
        /// If a system error is being encountered.
        /// </summary>
        SystemError
    }

    /// <summary>
    /// Represent a process debug monitor.
    /// </summary>
    public interface IDebugMonitor
    {
        /// <summary>
        /// Return the running processes on the debug monitor.
        /// </summary>
        /// <returns></returns>
        ProcessInfo[] GetRunningProcesses();

        /// <summary>
        /// Create the given process, then attach to it.
        /// </summary>
        /// <param name="commandLine"></param>
        /// <param name="initialDirectory"></param>
        void CreateAndAttachToProcess(string commandLine, string initialDirectory);

        /// <summary>
        /// Attach to a process using the given image name.
        /// </summary>
        /// <param name="processName">The name of the process to attach to.</param>
        void AttachToProcessByName(string processName);

        /// <summary>
        /// Detach to a process using the given id.
        /// </summary>
        /// <param name="processId">The ID of the process to attach to.</param>
        void AttachToProcessById(int processId);

        /// <summary>
        /// Begin logging debugger output to the specified path.
        /// </summary>
        /// <param name="listener">The output listener.</param>
        void AddOutputListener(IDebugOutputListener listener);

        /// <summary>
        /// Add a trigger listener to the debugger.
        /// </summary>
        /// <param name="triggerListener">The trigger listener.</param>
        void AddTriggerListener(IDebugEventListener triggerListener);

        /// <summary>
        /// Get the debugger status.
        /// </summary>
        /// <returns>The last status of the debugger.</returns>
        LastDebuggerStatus GetDebuggerStatus();

        /// <summary>
        /// Gracefully terminate the debugger, waiting for the session to end before returning.
        /// </summary>
        /// <param name="force">Force session termination immediately, possibly causing undefined behavior on the debuggee.</param>
        void EndSession(bool force);
    }
}
