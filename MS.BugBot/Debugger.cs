using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MS.BugBot
{
    using Interfaces;
    using Service;

    /// <summary>
    /// Represent a debugger factory interface.
    /// </summary>
    public static class Debugger
    {
        /// <summary>
        /// Create a monitor which can debug process on the same machine as this.
        /// </summary>
        /// <param name="symbols">The symbol paths to load.</param>
        /// <returns>A debug monitor task object.</returns>
        public static Task<IDebugMonitor> CreateUserMode(string[] symbols)
        {
            return Task.Run<IDebugMonitor>(() => {
                DebugMonitor ret = new DebugMonitor();
                ret.SetSymbols(symbols);
                ret.ShouldDispose += Ret_ShouldDispose;
                return ret;
            });
        }

        /// <summary>
        /// Connect to a UM process server and return a monitor object for the target device.
        /// </summary>
        /// <param name="connStr">The remote connection string.</param>
        /// <param name="symbols">The symbol paths to load.</param>
        /// <returns>A debug monitor object.</returns>
        /// <remarks>The connection method is analogous to a smart pipe.</remarks>
        public static Task<IDebugMonitor> ConnectToDebugServer(string connStr, string[] symbols)
        {
            return Task.Run<IDebugMonitor>(() => {
                DebugMonitor ret = new DebugMonitor();
                ret.SetSymbols(symbols);
                ret.ConnectToProcessServer(connStr, false);
                ret.ShouldDispose += Ret_ShouldDispose;
                return ret;
            });
        }

        /// <summary>
        /// Connect to a KM debugger and return a monitor object for the target device.
        /// </summary>
        /// <param name="connStr">The remote connection string.</param>
        /// <param name="symbols">The symbol path to load.</param>
        /// <returns>A debug monitor object.</returns>
        public static Task<IDebugMonitor> ConnectKernel(string connStr, string[] symbols)
        {
            return Task.Run<IDebugMonitor>(() => { 
                DebugMonitor ret = new DebugMonitor();
                ret.SetSymbols(symbols);
                ret.ConnectToProcessServer(connStr, true);
                ret.ShouldDispose += Ret_ShouldDispose;
                return ret;
            });
        }

        private static void Ret_ShouldDispose(object sender, EventArgs e)
        {
            if (sender != null)
            {
                DebugMonitor mon = (DebugMonitor)sender;
                mon.ShouldDispose -= Ret_ShouldDispose;
                mon.Dispose();
            }
        }
    }
}
