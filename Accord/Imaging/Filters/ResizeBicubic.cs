using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using LucasAlias.NINA.CGPUNINA.Image.ImageAnalysis;
using NINA.Core.Utility.Notification;
using NINA.CustomControlLibrary;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_ResizeBicubic")]
    [HarmonyPatch(typeof(ResizeBicubic), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    internal class Patch_ResizeBicubic_ProcessFilter {
        static bool Prefix(UnmanagedImage sourceData, UnmanagedImage destinationData) {
            if (CGPUNINAMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__OpCL != null && CGPUNINAMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__OpCL_Context is uint context) {
                Patch_ResizeBicubic.ProcessFilterOpenCL(ref sourceData, ref destinationData, CGPUNINAMediator.OpenCLManager, context);
            } else Patch_ResizeBicubic.ProcessFilter(ref sourceData, ref destinationData, CGPUNINAMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__MT);
            return false;
        }
    }
}
