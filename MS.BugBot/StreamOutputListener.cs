using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace MS.BugBot
{
    using Interfaces;
    using MS.Debuggers.DbgEng;


    /// <summary>
    /// Represent logging options for the debug monitor.
    /// </summary>
    [Flags]
    public enum LogOptions
    {
        /// <summary>
        /// Normal logging.
        /// </summary>
        Normal = 0x0,

        /// <summary>
        /// Include a timestamp in the debugger output.
        /// </summary>
        TimeStamp = 0x2,

        /// <summary>
        /// Include a datestamp in the debugger output.
        /// </summary>
        DateStamp = 0x4,

        /// <summary>
        /// All logging options.
        /// </summary>
        Default = TimeStamp | DateStamp
    }

    /// <summary>
    /// Represent a simple stream based output listener with timestamps.
    /// </summary>
    public class StreamOutputListener : IDebugOutputListener, IDisposable
    {
        StreamWriter _sw;
        LogOptions _options;
        int _written = 0;
        bool _start = true;
        bool _disposed = false;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="s">The stream to record output to.</param>
        /// <param name="options">The output options.</param>
        public StreamOutputListener(Stream s, LogOptions options)
        {
            _sw = new StreamWriter(s);
            _options = options;
        }

        /// <summary>
        /// Output handler.
        /// </summary>
        /// <param name="when">When the output occurred.</param>
        /// <param name="flags">The output flags.</param>
        /// <param name="text">The output text.</param>
        public void Output(DateTime when, OutputFlags flags, string text)
        {
            for (int i = 0; i < text.Length; ++i)
            {
                if (text[i] == '\r' || text[i] == '\n')
                {
                    if (i + 1 < text.Length && text[i + 1] == '\n')
                    {
                        ++i;
                    }
                    _start = true;
                    if (_written == 0)
                    {
                        AppendTimestamp(when);
                    }
                    _sw.WriteLine();
                    _written = 0;
                }
                else 
                {
                    if (_start)
                    {
                        _start = false;
                        AppendTimestamp(when);
                    }
                    _sw.Write(text[i]);
                    ++_written;
                }
            }
            _sw.Flush();
        }

        /// <summary>
        /// Disposer.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Protected disposer.
        /// </summary>
        /// <param name="disposing"></param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    if (_sw != null)
                    {
                        _sw.Close();
                        _sw.Dispose();
                        _sw = null;
                    }
                }

                _disposed = true;
            }
        }

        private void AppendTimestamp(DateTime when)
        {
            if (_options.HasFlag(LogOptions.DateStamp))
            {
                _sw.Write("{0:D4}/{1:D2}/{2:D2},", when.Year, when.Month, when.Day);
            }
            if (_options.HasFlag(LogOptions.TimeStamp))
            {
                _sw.Write("{0:D2}:{1:D2}:{2:D2}.{3:D3},", when.Hour, when.Minute, when.Second, when.Millisecond);
            }
        }

        /// <summary>
        /// Finalize this object.
        /// </summary>
        ~StreamOutputListener()
        {
            Dispose(false);
        }
    }
}
