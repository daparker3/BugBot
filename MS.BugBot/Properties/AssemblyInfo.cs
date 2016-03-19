using System;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security.Permissions;

[assembly:AssemblyCompanyAttribute("MS Internal")]
[assembly:AssemblyProductAttribute("BugBot")]
[assembly:AssemblyCopyrightAttribute("Copyright (c) MS 2015")]
[assembly:AssemblyCultureAttribute("en")]
[assembly:AssemblyVersionAttribute("1.0.0.0")]
[assembly:AssemblyFileVersionAttribute("1.0.0.0")]

//
// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute("MS.BugBot")]

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers
// by using the '*' as shown below:

[assembly:ComVisible(false)]

[assembly:CLSCompliantAttribute(true)]

[assembly:InternalsVisibleTo("MS.BugBot.Tests")]
[assembly: InternalsVisibleTo("MS.BugBot.Explorables")]

