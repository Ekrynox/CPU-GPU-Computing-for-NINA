using Accord.Imaging;
using HarmonyLib;
using NINA.Core.Utility;
using NINA.Core.Utility.Notification;
using NINA.CustomControlLibrary;
using NINA.Image.ImageAnalysis;
using NINA.Image.ImageData;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Image.ImageAnalysis {

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_BayerFilter16bpp")]
    [HarmonyPatch(typeof(BayerFilter16bpp), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    class Patch_BayerFilter16bpp_ProcessFilter {
        static bool Prefix (BayerFilter16bpp __instance, UnmanagedImage sourceData, UnmanagedImage destinationData) {
            LRGBArrays arr = __instance.LRGBArrays;
            if (CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL != null && CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL_Context is uint context) {
                try {
                    Patch_BayerFilter16bpp.ProcessFilterOpenCL(ref sourceData, ref destinationData, ref arr, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing, CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT, CGPUNINAMediator.OpenCLManager, context);
                } 
                catch (Exception e) {
                    Logger.Error($"CGPUNINA - BayerFilter16bpp - OpenCL execution error:\n{e.Message}");
                    Notification.ShowWarning("CGPUNINA: BayerFilter16bpp execution failed. Fallback to CPU.");
                    Patch_BayerFilter16bpp.ProcessFilter(ref sourceData, ref destinationData, ref arr, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing, CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT);
                }
            } else Patch_BayerFilter16bpp.ProcessFilter(ref sourceData, ref destinationData, ref arr, __instance.BayerPattern, __instance.SaveColorChannels, __instance.SaveLumChannel, __instance.PerformDemosaicing, CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT);
            __instance.LRGBArrays = arr;
            return false;
        }
    }

}
