using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_CannyEdgeDetector")]
    [HarmonyPatch(typeof(CannyEdgeDetector), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage), typeof(Rectangle) })]
    internal class Patch_CannyEdgeDetector_ProcessFilter {
        static bool Prefix(CannyEdgeDetector __instance, UnmanagedImage sourceData, UnmanagedImage destinationData, Rectangle rect) {
            var gaussianFilter = (GaussianBlur)AccessTools.Field(typeof(CannyEdgeDetector), "gaussianFilter").GetValue(__instance);
            Patch_CannyEdgeDetector.ProcessFilter(ref sourceData, ref destinationData, ref rect, ref gaussianFilter, __instance.LowThreshold, __instance.HighThreshold);
            AccessTools.Field(typeof(CannyEdgeDetector), "gaussianFilter").SetValue(__instance, gaussianFilter);
            return false;
        }
    }
}
