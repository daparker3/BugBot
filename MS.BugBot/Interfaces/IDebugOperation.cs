using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MS.Debuggers.DbgEng;

namespace MS.BugBot.Interfaces
{
    interface IDebugOperation
    {
        void Start(Debuggee debuggee);

        void Stop();
    }
}
