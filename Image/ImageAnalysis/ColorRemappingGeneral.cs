using Accord.Imaging;
using HarmonyLib;
using NINA.Image.ImageAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Image.ImageAnalysis {

    [HarmonyPatch(typeof(ColorRemappingGeneral), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(System.Drawing.Rectangle) })]
    internal class Patch_ColorRemappingGeneral_ProcessFilter {
        static bool Prefix(BayerFilter16bpp __instance, UnmanagedImage image, System.Drawing.Rectangle rect) {
            Patch_ColorRemappingGeneral.ProcessFilter(image, rect,
                (ushort[])AccessTools.Field(typeof(ColorRemappingGeneral), "redMap").GetValue(__instance),
                (ushort[])AccessTools.Field(typeof(ColorRemappingGeneral), "greenMap").GetValue(__instance),
                (ushort[])AccessTools.Field(typeof(ColorRemappingGeneral), "blueMap").GetValue(__instance),
                (ushort[])AccessTools.Field(typeof(ColorRemappingGeneral), "grayMap").GetValue(__instance));
            return false;
        }
    }

}
