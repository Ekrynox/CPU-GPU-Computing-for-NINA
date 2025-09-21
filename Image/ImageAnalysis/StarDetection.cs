using Accord.Imaging;
using HarmonyLib;
using LucasAlias.NINA.NinaPP.Image.ImageAnalysis.CPPStarDetection;
using NINA.Image.ImageAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using static NINA.Image.ImageAnalysis.StarDetection;

namespace LucasAlias.NINA.NinaPP.Image.ImageAnalysis {

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_StarDetection")]
    [HarmonyPatch(typeof(StarDetection.Star), nameof(StarDetection.Star.Calculate), new Type[] { typeof(List<PixelData>) })]
    internal class Patch_StarDetection_Star_Calculate {
        static readonly FieldInfo RectangleBacking = AccessTools.DeclaredField(typeof(StarDetection.Star), $"<{nameof(StarDetection.Star.Rectangle)}>k__BackingField");
        static readonly FieldInfo AverageBacking = AccessTools.DeclaredField(typeof(StarDetection.Star), $"<{nameof(StarDetection.Star.Average)}>k__BackingField");
        static readonly FieldInfo HFRBacking = AccessTools.DeclaredField(typeof(StarDetection.Star), $"<{nameof(StarDetection.Star.HFR)}>k__BackingField");

        static bool Prefix(StarDetection.Star __instance, List<PixelData> pixelData) {
            var Position = __instance.Position;
            var Rectangle = __instance.Rectangle;
            var Average = __instance.Average;
            var HFR = __instance.HFR;

            Patch_StarDetection.Patch_Star.Calculate(ref pixelData, ref Position, ref Rectangle, ref Average, ref HFR, __instance.Radius, __instance.SurroundingMean);

            __instance.Position = Position;
            RectangleBacking.SetValue(__instance, Rectangle);
            AverageBacking.SetValue(__instance, Average);
            HFRBacking.SetValue(__instance, HFR);
            return false;
        }
    }

}
