using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging.Filters {
    [HarmonyPatch(typeof(ResizeBicubic), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    internal class Patch_ResizeBicubic_ProcessFilter {
        static bool Prefix(ResizeBicubic __instance, UnmanagedImage sourceData, UnmanagedImage destinationData) {
            Patch_ResizeBicubic.ProcessFilter(ref sourceData, ref destinationData);
            return false;
        }
    }
}
