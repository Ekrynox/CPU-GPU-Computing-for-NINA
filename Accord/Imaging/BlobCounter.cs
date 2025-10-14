using Accord.Imaging;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Accord.Imaging {
    [HarmonyPatchCategory("Accord_Imaging_BlobCounter")]
    [HarmonyPatch(typeof(BlobCounter), "BuildObjectsMap", new Type[] { typeof(UnmanagedImage) })]
    internal class Patch_BlobCounter_BuildObjectsMap {
        static readonly FieldInfo ImageWidthBacking = AccessTools.DeclaredField(typeof(BlobCounterBase), $"<ImageWidth>k__BackingField");
        static readonly FieldInfo ImageHeightBacking = AccessTools.DeclaredField(typeof(BlobCounterBase), $"<ImageHeight>k__BackingField");

        static bool Prefix(BlobCounter __instance, UnmanagedImage image) {
            var ObjectLabels = (int[])AccessTools.Field(typeof(BlobCounterBase), "objectLabels").GetValue(__instance);
            var ObjectsCount = (int)AccessTools.Field(typeof(BlobCounterBase), "objectsCount").GetValue(__instance);

            Patch_BlobCounter.BuildObjectsMap(ref image,
                (int)ImageWidthBacking.GetValue(__instance),
                (int)ImageHeightBacking.GetValue(__instance),
                ref ObjectLabels, ref ObjectsCount,
                __instance.BackgroundThreshold.R, __instance.BackgroundThreshold.G, __instance.BackgroundThreshold.B
                );

            AccessTools.Field(typeof(BlobCounterBase), "objectLabels").SetValue(__instance, ObjectLabels);
            AccessTools.Field(typeof(BlobCounterBase), "objectsCount").SetValue(__instance, ObjectsCount);
            return false;
        }
    }
}
