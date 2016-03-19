using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace MS.BugBot.Service
{
    using Interfaces;
    using Debuggers.DbgEng;

    class DebugMonitorContext : IDebugContext
    {
        private Debuggee _debugee;
        private StringBuilder _output = new StringBuilder();

        public DebugMonitorContext(Debuggee debugee)
        {
            this._debugee = debugee;
        }

        public string RunCommand(string command)
        {
            _debugee.DebugOutput += _debugee_DebugOutput;

            try
            {
                _output.Clear();
                _debugee.DebugControl.Execute(DebugOutputControl.ThisClient, command, DebugExecuteFlags.Echo | DebugExecuteFlags.NoRepeat);
                return _output.ToString();
            }
            catch (COMException ce)
            {
                throw new DebugMonitorException("Command failed.", ce);
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Command failed.", ic);
            }
            finally
            {
                _debugee.DebugOutput -= _debugee_DebugOutput;
            }
        }

        public void SaveDumpFile(string fileName, DumpType dumpType, DumpFlags dumpFlags, string dumpDescription)
        {
            try
            {
                _debugee.WriteDumpFile(fileName, dumpType, dumpFlags, dumpDescription);
            }
            catch (COMException ce)
            {
                throw new DebugMonitorException("Dump failed.", ce);
            }
            catch (InvalidCastException ic)
            {
                throw new DebugMonitorException("Dump failed.", ic);
            }
        }

        private void _debugee_DebugOutput(object sender, DebugOutputEventArgs e)
        {
            _output.Append(e.Output);
        }
    }
}
