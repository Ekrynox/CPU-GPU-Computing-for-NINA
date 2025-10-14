using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using NINA.Image.ImageAnalysis;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_NoBlurCannyEdgeDetector")]
    [HarmonyPatch(typeof(NoBlurCannyEdgeDetector), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage), typeof(Rectangle) })]
    internal class Patch_NoBlurCannyEdgeDetector_ProcessFilter {
        static bool Prefix(NoBlurCannyEdgeDetector __instance, UnmanagedImage source, UnmanagedImage destination, Rectangle rect) {
            Patch_NoBlurCannyEdgeDetector.ProcessFilter(ref source, ref destination, ref rect, __instance.LowThreshold, __instance.HighThreshold);
            return false;
        }
    }
}
