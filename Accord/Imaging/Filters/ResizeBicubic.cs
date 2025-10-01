using Accord.Imaging;
using Accord.Imaging.Filters;
using HarmonyLib;
using LucasAlias.NINA.NinaPP.Image.ImageAnalysis;
using NINA.Core.Utility.Notification;
using NINA.CustomControlLibrary;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_ResizeBicubic")]
    [HarmonyPatch(typeof(ResizeBicubic), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage) })]
    internal class Patch_ResizeBicubic_ProcessFilter {
        static bool Prefix(UnmanagedImage sourceData, UnmanagedImage destinationData) {
            Stopwatch sw = Stopwatch.StartNew();
            for (int i = 0; i < 100; i++) {
                if (NinaPPMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__OpCL != null && NinaPPMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__OpCL_Context is uint context) {
                    Patch_ResizeBicubic.ProcessFilterOpenCL(ref sourceData, ref destinationData, NinaPPMediator.OpenCLManager, context);
                } else Patch_ResizeBicubic.ProcessFilter(ref sourceData, ref destinationData, NinaPPMediator.Plugin.Accord_Imaging_Filters_ResizeBicubic__MT);
            }
            sw.Stop();
            Notification.ShowInformation($"Average time (ms): {sw.Elapsed.TotalMilliseconds / 100}");
            return false;
        }
    }
}
