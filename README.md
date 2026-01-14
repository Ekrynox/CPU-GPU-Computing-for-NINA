# CPU\GPU Computing for N.I.N.A
This is a [N.I.N.A.](https://github.com/isbeorn/nina) plugin that aims to move performance-hungry functions to a compiled C++ code.

It works by injecting C# replacement functions that link to C++ code, with [Harmony](https://harmony.pardeike.net/).

My work on this plugin mainly consists of the Injection scripts, OpenCL Management, and some Optimizations.
A large portion of the code is just an translation of the original N.I.N.A and Accord.NET algorithms. Hence those C++ translation remains under the licenses of their sources. A updated copy of the different licenses is available at the top of concerned files.

This plugin allow great performance gains on smaller configurations, with no impact on results.

Example: Ultrabook with an Intel I5 6200U (2c/4t) + 4Gb DDR4 => arround 2.5 times reduction on snashot pipeline (debayering & stars detections) => from arround 17s to 6s on a Nikon Z6II 24mp raw shot for 5000+ stars.

## Installation
This plugin requires [Microsoft Visual C++ Redistributable X64](https://aka.ms/vs/17/release/vc_redist.x64.exe).  
If not installed, you will encounter a missing "CGPUNINA.Wrapper" or "CGPUNINA.Native" error.

Then, you can install NINA++ through the Plugin section in N.I.N.A. or by extracting the archive from the Release in the plugin folder: %localappdata%\NINA\Plugins.

## Licensing
This project includes modifications and translations of code from [N.I.N.A.](https://github.com/isbeorn/nina), which is licensed under the Mozilla Public License 2.0.
Files containing adapted [N.I.N.A.](https://github.com/isbeorn/nina) code remain under the MPL-2.0.

This project includes modifications and translations of code from [Accord.NET](http://accord-framework.net/), which is licensed under the GNU Lesser General Public License v2.1.
Files containing adapted [Accord.NET](http://accord-framework.net/) code remain under the LGPL-2.1.
