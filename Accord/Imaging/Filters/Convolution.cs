using Accord.Imaging;
using Accord.Imaging.Filters;
using Accord.Statistics.Kernels;
using HarmonyLib;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace LucasAlias.NINA.NinaPP.Accord.Imaging.Filters {
    [HarmonyPatchCategory("Accord_Imaging_Filters_Convolution")]
    [HarmonyPatch(typeof(Convolution), "ProcessFilter", new Type[] { typeof(UnmanagedImage), typeof(UnmanagedImage), typeof(Rectangle) })]
    internal class Patch_Convolution_ProcessFilter {
        static bool Prefix(Convolution __instance, UnmanagedImage sourceData, UnmanagedImage destinationData, Rectangle rect) {
            Patch_Convolution.ProcessFilter(ref sourceData, ref destinationData, ref rect, __instance.ProcessAlpha, __instance.Kernel, __instance.Divisor, __instance.Threshold, __instance.Kernel.GetLength(1), __instance.DynamicDivisorForEdges, NinaPPMediator.Plugin.Accord_Imaging_Filters_Convolution__MT);
            return false;
        }
    };
}
