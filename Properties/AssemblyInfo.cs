using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

// [MANDATORY] The following GUID is used as a unique identifier of the plugin. Generate a fresh one for your plugin!
[assembly: Guid("b2ccb2de-7f7d-40be-915f-96ce68df38fa")]

// [MANDATORY] The assembly versioning
//Should be incremented for each new release build of a plugin
[assembly: AssemblyVersion("0.5.1.0")]
[assembly: AssemblyFileVersion("0.5.1.0")]

// [MANDATORY] The name of your plugin
[assembly: AssemblyTitle("CPU\\GPU Computing for NINA")]
// [MANDATORY] A short description of your plugin
[assembly: AssemblyDescription("A NINA Plugin that move some N.I.N.A & Accord heavy functions inside native code and GPU to speed it up.")]

// The following attributes are not required for the plugin per se, but are required by the official manifest meta data

// Your name
[assembly: AssemblyCompany("Lucas Alias")]
// The product name that this plugin is part of
[assembly: AssemblyProduct("CGPUNINA")]
[assembly: AssemblyCopyright("Copyright © 2025 Lucas Alias")]

// The minimum Version of N.I.N.A. that this plugin is compatible with
[assembly: AssemblyMetadata("MinimumApplicationVersion", "3.2.0.3011")]

// The license your plugin code is using
[assembly: AssemblyMetadata("License", "MPL-2.0")]
// The url to the license
[assembly: AssemblyMetadata("LicenseURL", "https://www.mozilla.org/en-US/MPL/2.0/")]
// The repository where your pluggin is hosted
[assembly: AssemblyMetadata("Repository", "https://github.com/Ekrynox/CPU-GPU-Computing-for-NINA")]

// The following attributes are optional for the official manifest meta data

//[Optional] Your plugin homepage URL - omit if not applicaple
[assembly: AssemblyMetadata("Homepage", "https://github.com/Ekrynox/CPU-GPU-Computing-for-NINA")]

//[Optional] Common tags that quickly describe your plugin
[assembly: AssemblyMetadata("Tags", "Optimization")]

//[Optional] A link that will show a log of all changes in between your plugin's versions
[assembly: AssemblyMetadata("ChangelogURL", "https://github.com/Ekrynox/CPU-GPU-Computing-for-NINA/blob/main/CHANGELOG.md")]

//[Optional] The url to a featured logo that will be displayed in the plugin list next to the name
[assembly: AssemblyMetadata("FeaturedImageURL", "")]
//[Optional] A url to an example screenshot of your plugin in action
[assembly: AssemblyMetadata("ScreenshotURL", "")]
//[Optional] An additional url to an example example screenshot of your plugin in action
[assembly: AssemblyMetadata("AltScreenshotURL", "")]
//[Optional] An in-depth description of your plugin
[assembly: AssemblyMetadata("LongDescription", @"
# CPU\GPU Computing for N.I.N.A
This is a [N.I.N.A.](https://github.com/isbeorn/nina) plugin that aims to move performance-hungry functions to a compiled C++ code.

It works by injecting C# replacement functions that link to C++ code, with [Harmony](https://harmony.pardeike.net/).

My work on this plugin mainly consists of the Injection scripts, OpenCL Management, and some Optimizations.
A large portion of the code is just an translation of the original N.I.N.A and Accord.NET algorithms. Hence those C++ translation remains under the licenses of their sources. A updated copy of the different licenses is available at the top of concerned files.

This plugin allow great performance gains on smaller configurations, with no impact on results.

Example: Ultrabook with an Intel I5 6200U (2c/4t) + 4Gb DDR4 => arround 2.5 times reduction on snashot pipeline (debayering & stars detections) => from arround 17s to 6s on a Nikon Z6II 24mp raw shot for 5000+ stars.
")]

// Setting ComVisible to false makes the types in this assembly not visible
// to COM components.  If you need to access a type in this assembly from
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]
// [Unused]
[assembly: AssemblyConfiguration("")]
// [Unused]
[assembly: AssemblyTrademark("")]
// [Unused]
[assembly: AssemblyCulture("")]