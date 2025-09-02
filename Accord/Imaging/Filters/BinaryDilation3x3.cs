using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_BinaryDilation3x3")]
    [HarmonyPatch(typeof(BinaryDilation3x3), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage), typeof(Rectangle) })]
    internal class Patch_BinaryDilation3x3_ProcessFilter {
        static bool Prefix(UnmanagedImage sourceData, UnmanagedImage destinationData, Rectangle rect) {
            Patch_BinaryDilation3x3.ProcessFilter(ref sourceData, ref destinationData, ref rect);
            return false;
        }
    }
}
