using System.Collections.Generic;
using MS.BugBot.Interfaces;
using MS.Debuggers.DbgEng;
using System;
using MS.BugBot;
using Microsoft.Pex.Framework;
using Microsoft.Pex.Framework.Validation;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MS.BugBot.Tests
{
    /// <summary>This class contains parameterized unit tests for FileCrashMonitor</summary>
    [TestClass]
    [PexClass(typeof(FileCrashMonitor))]
    [PexAllowedExceptionFromTypeUnderTest(typeof(ArgumentException), AcceptExceptionSubtypes = true)]
    [PexAllowedExceptionFromTypeUnderTest(typeof(InvalidOperationException))]
    public partial class FileCrashMonitorTest
    {

        /// <summary>Test stub for .ctor(String, Int32, DumpFlags)</summary>
        [PexMethod]
        public FileCrashMonitor ConstructorTest(
            string fileName,
            int maxCrashes,
            DumpFlags dumpFlags
        )
        {
            FileCrashMonitor target = new FileCrashMonitor(fileName, maxCrashes, dumpFlags);
            return target;
            // TODO: add assertions to method FileCrashMonitorTest.ConstructorTest(String, Int32, DumpFlags)
        }

        /// <summary>Test stub for HandleEvent(DebugTrigger, DebuggerEventArgs, IDebugContext)</summary>
        [PexMethod]
        public void HandleEventTest(
            [PexAssumeUnderTest]FileCrashMonitor target,
            DebugTrigger triggerType,
            DebuggerEventArgs eventArgs,
            IDebugContext context
        )
        {
            target.HandleEvent(triggerType, eventArgs, context);
            // TODO: add assertions to method FileCrashMonitorTest.HandleEventTest(FileCrashMonitor, DebugTrigger, DebuggerEventArgs, IDebugContext)
        }

        /// <summary>Test stub for HandlesEvent(DebugTrigger, DebuggerEventArgs)</summary>
        [PexMethod]
        public bool HandlesEventTest(
            [PexAssumeUnderTest]FileCrashMonitor target,
            DebugTrigger triggerType,
            DebuggerEventArgs eventArgs
        )
        {
            bool result = target.HandlesEvent(triggerType, eventArgs);
            return result;
            // TODO: add assertions to method FileCrashMonitorTest.HandlesEventTest(FileCrashMonitor, DebugTrigger, DebuggerEventArgs)
        }

        /// <summary>Test stub for get_CrashesRecorded()</summary>
        [PexMethod]
        public ICollection<string> CrashesRecordedGetTest([PexAssumeUnderTest]FileCrashMonitor target)
        {
            ICollection<string> result = target.CrashesRecorded;
            return result;
            // TODO: add assertions to method FileCrashMonitorTest.CrashesRecordedGetTest(FileCrashMonitor)
        }
    }
}
