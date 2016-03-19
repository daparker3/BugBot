using System;
using System.Collections.Generic;
using System.IO;

using EnsureThat;

namespace MS.BugBot
{
    using Debuggers.DbgEng;

    using Interfaces;

    /// <summary>
    /// Represent a stream-based crash monitor.
    /// </summary>
    public class FileCrashMonitor : IDebugEventListener
    {
        string _fileName;
        List<string> _crashesRecorded = new List<string>();
        int _maxCrashes;
        DumpFlags _dumpFlags;

        /// <summary>
        /// Return the crash dumps recorded during this session.
        /// </summary>
        public ICollection<string> CrashesRecorded
        {
            get
            {
                return _crashesRecorded;
            }
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="fileName">The name of the file to record crash dumps to.</param>
        /// <param name="maxCrashes">The maximum number of dumps to record before giving up.</param>
        /// <param name="dumpFlags">The dump flags.</param>
        public FileCrashMonitor(string fileName, int maxCrashes, DumpFlags dumpFlags)
        {
            Ensure.That(fileName, "fileName").IsNotNullOrEmpty();

            _fileName = fileName;
            _maxCrashes = maxCrashes;
            _dumpFlags = dumpFlags;
        }

        /// <summary>
        /// Handle a trigger event.
        /// </summary>
        /// <param name="triggerType">The type of trigger to handle.</param>
        /// <param name="eventArgs">The trigger event arguments.</param>
        /// <returns>True if the trigger should be handled.</returns>
        public bool HandlesEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs)
        {
            return triggerType == DebugTrigger.Exception && !((ExceptionEventArgs)eventArgs).FirstChance;
        }

        /// <summary>
        /// Handle a trigger event.
        /// </summary>
        /// <param name="triggerType">The type of trigger to handle.</param>
        /// <param name="eventArgs">The trigger event arguments.</param>
        /// <param name="context">The debug context to perform.</param>
        /// <returns>True if the trigger should be handled.</returns>
        public void HandleEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs, IDebugContext context)
        {
            if (triggerType == DebugTrigger.Exception && !((ExceptionEventArgs)eventArgs).FirstChance)
            {
                try
                {
                    GenerateExceptionReport(context);
                }
                catch (DebugMonitorException)
                {
                }
            }
        }

        private void GenerateExceptionReport(IDebugContext context)
        {
            string tFn = _fileName;

            if (_maxCrashes > 0 && _crashesRecorded.Count > _maxCrashes)
            {
                return;
            }
            else if (_crashesRecorded.Count > 0)
            {
                string path = Path.GetDirectoryName(_fileName);
                string name = Path.GetFileNameWithoutExtension(_fileName);
                string ext = Path.GetExtension(_fileName);

                tFn = Path.Combine(path, name + _crashesRecorded + ext);

                if (File.Exists(tFn))
                {
                    File.Delete(tFn);
                }
            }

            context.SaveDumpFile(tFn, DumpType.Small, _dumpFlags, "Auto-generated from crash monitor.");
            _crashesRecorded.Add(tFn);
        }
    }
}
