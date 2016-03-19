using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MS.Debuggers.DbgEng;

namespace MS.BugBot.Interfaces
{
    /// <summary>
    /// Represent a debug output listener.
    /// </summary>
    public interface IDebugOutputListener
    {
        /// <summary>
        /// Output a line of text from the debugger.
        /// </summary>
        /// <param name="when">When did the output occur.</param>
        /// <param name="flags">Any output flags.</param>
        /// <param name="text">The text to output.</param>
        void Output(DateTime when, OutputFlags flags, string text);
    }
}
