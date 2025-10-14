# CPU\GPU Computing for NINA
This is a [N.I.N.A.](https://github.com/isbeorn/nina) plugin that aims to move performance-hungry functions to a compiled C++ code.

It works by injecting C# replacement functions that link to C++ code, with [Harmony](https://harmony.pardeike.net/).

Several files of source code of this plugin originates from N.I.N.A and Accord Framework.
I only copied it from C# to C++, did the few needed adjustements, and make the code to inject the new native functions inside N.I.N.A with Harmony.
Now, this plugin also offer an GPU Implementation of some functions with OpenCL.

Hence almost all files from this project remains under the licenses of their sources. A updated copy of the different licenses is available at the top of concerned files.

This plugin allow great performance gains on smaller configurations, with no impact on results.

Example: Ultrabook with an Intel I5 6200U (2c/4t) + 4Gb DDR4 => arround 2.5 times reduction on snashot pipeline (debayering & stars detections) => from arround 17s to 6s on a Nikon Z6II 24mp raw shot for 5000+ stars.

## Installation
This plugin requires [VC Redist X64](https://aka.ms/vs/17/release/vc_redist.x64.exe).
You can then install NINA++ through the Plugin section in N.I.N.A. or by extracting the archive from the Release in the plugin folder: %localappdata%\NINA\Plugins.

## Licensing
This project includes modifications and translations of code from [N.I.N.A.](https://github.com/isbeorn/nina), which is licensed under the Mozilla Public License 2.0.
Files containing adapted [N.I.N.A.](https://github.com/isbeorn/nina) code remain under the MPL-2.0.

This project includes modifications and translations of code from [Accord.NET](http://accord-framework.net/), which is licensed under the GNU Lesser General Public License v2.1.
Files containing adapted [Accord.NET](http://accord-framework.net/) code remain under the LGPL-2.1.
