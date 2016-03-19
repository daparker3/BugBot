using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;

namespace MS.BugBot
{
    /// <summary>
    /// Represent a wrapped fault coming from the debug monitor service.
    /// </summary>
    [Serializable]
    public class DebugMonitorException : Exception
    {
        /// <summary>
        /// The action being performed when the exception happened.
        /// </summary>
        public string ActionName
        {
            get;
            private set;
        }        

        /// <summary>
        /// Constructor.
        /// </summary>
        public DebugMonitorException() { }
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="message"></param>
        public DebugMonitorException(string message) : base(message) { }
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="actionName"></param>
        /// <param name="message"></param>
        public DebugMonitorException(string actionName, string message) : base(message) 
        {
            ActionName = actionName;
        }
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="message"></param>
        /// <param name="inner"></param>
        public DebugMonitorException(string message, Exception inner) : base(message, inner) { }
        /// <summary>
        /// Serialization constructor.
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        protected DebugMonitorException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context)
        {
            ActionName = info.GetString("ActionName");
        }
        /// <summary>
        /// Deserializer.
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("ActionName", ActionName);
            base.GetObjectData(info, context);
        }
    }
}
