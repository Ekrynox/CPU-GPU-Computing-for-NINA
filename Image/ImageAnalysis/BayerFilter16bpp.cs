using Accord.Imaging;
using HarmonyLib;
using NINA.Image.ImageAnalysis;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Image.ImageAnalysis {

    [HarmonyPatch(typeof(BayerFilter16bpp), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    class Patch_BayerFilter16bpp_ProcessFilter {
        static bool Prefix (BayerFilter16bpp __instance, UnmanagedImage sourceData, UnmanagedImage destinationData) {
            Patch_BayerFilter16bpp.ProcessFilter(sourceData, destinationData, __instance.LRGBArrays, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing);
            return false;
        }
    }

}
