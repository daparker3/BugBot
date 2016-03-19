using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MS.Debuggers.DbgEng;

namespace MS.BugBot.Interfaces
{
    /// <summary>
    /// Represent a listener for specific debug events.
    /// </summary>
    public interface IDebugEventListener
    {
        /// <summary>
        /// Return if the listener will handle the given trigger.
        /// </summary>
        /// <param name="triggerType">The trigger being fired.</param>
        /// <param name="eventArgs">The event arguments being passed.</param>
        /// <returns>True if the trigger will be handled.</returns>
        bool HandlesEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs);

        /// <summary>
        /// Handle a debug trigger.
        /// </summary>
        /// <param name="triggerType">The trigger being fired.</param>
        /// <param name="eventArgs">The event arguments being passed.</param>
        /// <param name="context">The debug context which will handle the event.</param>
        void HandleEvent(DebugTrigger triggerType, DebuggerEventArgs eventArgs, IDebugContext context);
    }
}
