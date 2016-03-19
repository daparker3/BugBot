using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MS.BugBot.Interfaces
{
    using Debuggers.DbgEng;

    /// <summary>
    /// Represent a context through which a debuggee can analyze the debug session.
    /// </summary>
    public interface IDebugContext
    {
        /// <summary>
        /// Run a debugger command.
        /// </summary>
        /// <param name="command">The command being run.</param>
        /// <returns>A string representing the command output.</returns>
        string RunCommand(string command);

        /// <summary>
        /// Save a dump file to the specified location.
        /// </summary>
        /// <param name="dumpFile">The name of the dump file to write.</param>
        /// <param name="dumpType">The type of dump to write.</param>
        /// <param name="dumpFlags">The dump flags.</param>
        /// <param name="dumpDescription">The dump description.</param>
        void SaveDumpFile(string dumpFile, DumpType dumpType, DumpFlags dumpFlags, string dumpDescription);
    }
}
