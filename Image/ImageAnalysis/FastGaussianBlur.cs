using Accord.Imaging;
using HarmonyLib;
using NINA.Core.Utility;
using NINA.Core.Utility.Notification;
using NINA.CustomControlLibrary;
using NINA.Image.ImageAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA.Image.ImageAnalysis {

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur")]
    [HarmonyPatch(typeof(FastGaussianBlur), "gaussBlur_4", new Type[] { typeof(byte[]), typeof(byte[]), typeof(int) })]
    internal class Patch_FastGaussianBlur_gaussBlur_4 {
        static bool Prefix(FastGaussianBlur __instance, byte[] source, byte[] dest, int r) {
            if (CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_FastGaussianBlur__OpCL != null && CGPUNINAMediator.Plugin.NINA_Image_ImageAnalysis_FastGaussianBlur__OpCL_Context is uint context) {
                try {
                    Patch_FastGaussianBlur.gaussBlur_4OpenCL(source, dest, r,
                        (int)AccessTools.Field(typeof(FastGaussianBlur), "_width").GetValue(__instance),
                        (int)AccessTools.Field(typeof(FastGaussianBlur), "_height").GetValue(__instance),
                        CGPUNINAMediator.OpenCLManager, context
                        );
                } catch (Exception e) {
                    Logger.Error($"CGPUNINA - FastGaussianBlur - OpenCL execution error:\n{e.Message}");
                    Notification.ShowWarning("CGPUNINA: FastGaussianBlur execution failed. Fallback to CPU.");
                    Patch_FastGaussianBlur.gaussBlur_4(source, dest, r,
                        (int)AccessTools.Field(typeof(FastGaussianBlur), "_width").GetValue(__instance),
                        (int)AccessTools.Field(typeof(FastGaussianBlur), "_height").GetValue(__instance)
                        );
                }
            }
            else Patch_FastGaussianBlur.gaussBlur_4(source, dest, r,
                (int)AccessTools.Field(typeof(FastGaussianBlur), "_width").GetValue(__instance),
                (int)AccessTools.Field(typeof(FastGaussianBlur), "_height").GetValue(__instance)
                );
            return false;
        }
    }

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur")]
    [HarmonyPatch(typeof(FastGaussianBlur), "boxesForGauss", new Type[] { typeof(int), typeof(int) })]
    internal class Patch_FastGaussianBlur_boxesForGauss {
        static bool Prefix(int[] __result, int sigma, int n) {
            __result = Patch_FastGaussianBlur.boxesForGauss(sigma, n);
            return false;
        }
    }

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur")]
    [HarmonyPatch(typeof(FastGaussianBlur), "boxBlur_4", new Type[] { typeof(byte[]), typeof(byte[]), typeof(int), typeof(int), typeof(int) })]
    internal class Patch_FastGaussianBlur_boxBlur_4 {
        static bool Prefix(byte[] source, byte[] dest, int w, int h, int r) {
            Patch_FastGaussianBlur.boxBlur_4(source, dest, w, h, r);
            return false;
        }
    }

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur")]
    [HarmonyPatch(typeof(FastGaussianBlur), "boxBlurH_4", new Type[] { typeof(byte[]), typeof(byte[]), typeof(int), typeof(int), typeof(int) })]
    internal class Patch_FastGaussianBlur_boxBlurH_4 {
        static bool Prefix(byte[] source, byte[] dest, int w, int h, int r) {
            Patch_FastGaussianBlur.boxBlurH_4(source, dest, w, h, r);
            return false;
        }
    }

    [HarmonyPatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur")]
    [HarmonyPatch(typeof(FastGaussianBlur), "boxBlurT_4", new Type[] { typeof(byte[]), typeof(byte[]), typeof(int), typeof(int), typeof(int) })]
    internal class Patch_FastGaussianBlur_boxBlurT_4 {
        static bool Prefix(byte[] source, byte[] dest, int w, int h, int r) {
            Patch_FastGaussianBlur.boxBlurT_4(source, dest, w, h, r);
            return false;
        }
    }
}
