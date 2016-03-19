using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using EnsureThat;

namespace MS.BugBot
{
    using Interfaces;
    using MS.Debuggers.DbgEng;


    /// <summary>
    /// A trigger listener which runs a series of commands when encountering the given condition.
    /// </summary>
    public class SimpleTriggerListener : IDebugEventListener
    {
        DebugTrigger _trigger;
        string[] _debugScript;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="triggerType">The type of trigger to evaluate.</param>
        /// <param name="debugCommands">The debug commands to perform.</param>
        public SimpleTriggerListener(DebugTrigger triggerType, string[] debugCommands)
        {
            Ensure.That(debugCommands, "debugCommands").IsNotNull();
            _trigger = triggerType;
            _debugScript = debugCommands;
        }

        /// <summary>
        /// Handle the given trigger.
        /// </summary>
        /// <param name="triggerType">The type of trigger to evaluate.</param>
        /// <param name="eventArgs">The event arguments to handle.</param>
        /// <returns>True if the event should be handled.</returns>
        public bool HandlesEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs)
        {
            return _trigger == triggerType;
        }

        /// <summary>
        /// Handle the given trigger.
        /// </summary>
        /// <param name="triggerType">The type of trigger to evaluate.</param>
        /// <param name="eventArgs">The event arguments to handle.</param>
        /// <param name="context">The debug context to execute the action on.</param>
        public void HandleEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs, IDebugContext context)
        {
            if (_trigger == triggerType)
            {
                foreach (string command in _debugScript)
                {
                    try
                    {
                        context.RunCommand(command);
                    }
                    catch (DebugMonitorException)
                    {
                    }
                }
            }
        }
    }
}
