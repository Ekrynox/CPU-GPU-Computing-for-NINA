using Accord.Imaging;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging {
    [HarmonyPatch("Accord_Imaging_BlobCounterBase")]
    [HarmonyPatch(typeof(BlobCounterBase), "CollectObjectsInfo", new Type[] { typeof(UnmanagedImage) })]
    internal class Patch_BlobCounterBase_CollectObjectsInfo {
        static readonly FieldInfo ImageWidthBacking = AccessTools.DeclaredField(typeof(BlobCounterBase), $"<ImageWidth>k__BackingField");
        static readonly FieldInfo ImageHeightBacking = AccessTools.DeclaredField(typeof(BlobCounterBase), $"<ImageHeight>k__BackingField");

        static bool Prefix(BlobCounterBase __instance, UnmanagedImage image) {
            var ObjectLabels = (int[])AccessTools.Field(typeof(BlobCounterBase), "objectLabels").GetValue(__instance);
            var ObjectsCount = (int)AccessTools.Field(typeof(BlobCounterBase), "objectsCount").GetValue(__instance);
            var blobs = (List<Blob>)AccessTools.Field(typeof(BlobCounterBase), "blobs").GetValue(__instance);

            Patch_BlobCounterBases.CollectObjectsInfo(ref image,
                (int)ImageWidthBacking.GetValue(__instance),
                (int)ImageHeightBacking.GetValue(__instance),
                ref ObjectLabels, ObjectsCount, ref blobs
                );

            AccessTools.Field(typeof(BlobCounterBase), "objectLabels").SetValue(__instance, ObjectLabels);
            AccessTools.Field(typeof(BlobCounterBase), "objectsCount").SetValue(__instance, ObjectsCount);
            AccessTools.Field(typeof(BlobCounterBase), "blobs").SetValue(__instance, blobs);
            return false;
        }
    }
}
