using Accord.Imaging;
using HarmonyLib;
using NINA.CustomControlLibrary;
using NINA.Image.ImageAnalysis;
using NINA.Image.ImageData;
using NINA.Core.Utility.Notification;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Image.ImageAnalysis {

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_BayerFilter16bpp")]
    [HarmonyPatch(typeof(BayerFilter16bpp), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    class Patch_BayerFilter16bpp_ProcessFilter {
        static bool Prefix (BayerFilter16bpp __instance, UnmanagedImage sourceData, UnmanagedImage destinationData) {
            LRGBArrays arr = __instance.LRGBArrays;
            //Patch_BayerFilter16bpp.ProcessFilter(ref sourceData, ref destinationData, ref arr, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing, NinaPPMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT);
            Patch_BayerFilter16bpp.ProcessFilterOpenCL(ref sourceData, ref destinationData, ref arr, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing, NinaPPMediator.OpenCLManager, 0);
            __instance.LRGBArrays = arr;
            return false;
        }
    }

}
