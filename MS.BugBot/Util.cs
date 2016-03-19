using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace MS.BugBot
{
    static class Util
    {
        static class NativeMethods
        {
            [DllImport("kernel32.dll")]
            public static extern IntPtr GetCurrentProcess();

            [StructLayout(LayoutKind.Sequential)]
            public struct PROCESS_MEMORY_COUNTERS
            {
                public uint cb;
                public uint PageFaultCount;
                public ulong PeakWorkingSetSize;
                public ulong WorkingSetSize;
                public ulong QuotaPeakPagedPoolUsage;
                public ulong QuotaPagedPoolUsage;
                public ulong QuotaPeakNonPagedPoolUsage;
                public ulong QuotaNonPagedPoolUsage;
                public ulong PagefileUsage;
                public ulong PeakPagefileUsage;
            }

            [DllImport("psapi.dll", SetLastError = true)]
            public static extern bool GetProcessMemoryInfo(IntPtr hProcess, [In, Out] ref PROCESS_MEMORY_COUNTERS counters, uint size);
        }
        
        public static void FixLineBreaks(string fileName)
        {
            // Normalize text file line breaks.
            string tmpFile = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());

            using (StreamReader sr = new StreamReader(File.Open(fileName, FileMode.Open, FileAccess.ReadWrite, FileShare.Read)))
            {
                using (StreamWriter sw = new StreamWriter(tmpFile))
                {
                    while (!sr.EndOfStream)
                    {
                        char ch = (char)sr.Read();

                        if (ch == '\r')
                        {
                            int next = sr.Peek();

                            if (next >= 0 && (char)next == '\n')
                            {
                                sw.WriteLine();
                                continue;
                            }
                        }
                        if (ch == '\n')
                        {
                            int next = sr.Peek();

                            if (next >= 0 && (char)next == '\r')
                            {
                                sw.WriteLine();
                                continue;
                            }
                        }
                        if (ch == '\n')
                        {
                            sw.WriteLine();
                            continue;
                        }

                        sw.Write(ch);
                    }
                }
            }

            File.Copy(tmpFile, fileName, true);
            try
            {
                File.Delete(tmpFile);
            }
            catch
            {
                // Don't care if this happens or not.
            }
        }

        public static void LogMemoryUsageStatistics(string processStage)
        {
#if DEBUG
            IntPtr currentProcess = NativeMethods.GetCurrentProcess();
            NativeMethods.PROCESS_MEMORY_COUNTERS memoryCounters = new NativeMethods.PROCESS_MEMORY_COUNTERS();
            int memorySize = 72;
            memoryCounters.cb = (uint)memorySize;

            if (NativeMethods.GetProcessMemoryInfo(currentProcess, ref memoryCounters, (uint)memorySize))
            {
                Log.Info("  MEM: Stage: {0}", processStage);
                Log.Info("  MEM: WorkingSetSize: {0:F2}MB", (double)memoryCounters.WorkingSetSize / 1024.0 / 1024.0);
                Log.Info("  MEM: PagefileUsage: {0:F2}MB", (double)memoryCounters.PagefileUsage / 1024.0 / 1024.0);
            }
            else
            {
                Log.Info("GetProcessMemoryInfo(): {0}", Marshal.GetLastWin32Error());
            }
#endif
        }
    }
}
