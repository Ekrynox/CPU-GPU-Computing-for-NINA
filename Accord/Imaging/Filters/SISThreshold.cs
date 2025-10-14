using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_SISThreshold")]
    [HarmonyPatch(typeof(SISThreshold), nameof(SISThreshold.CalculateThreshold), new Type[] { typeof(UnmanagedImage), typeof(Rectangle) })]
    internal class Patch_SISThreshold_CalculateThreshold {
        static bool Prefix(SISThreshold __instance, ref int __result, UnmanagedImage image, Rectangle rect) {
            __result = Patch_SISThreshold.CalculateThreshold(ref image, ref rect);
            return false;
        }
    }
}
