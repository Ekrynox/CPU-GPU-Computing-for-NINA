using Accord.Imaging;
using HarmonyLib;
using NINA.Core.Enum;
using NINA.Core.Locale;
using NINA.Core.Utility;
using NINA.Image.ImageAnalysis;
using NINA.Image.ImageData;
using NINA.Image.Interfaces;
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace LucasAlias.NINA.NinaPP.Image.ImageAnalysis {

    [HarmonyPatch(typeof(ImageUtility),nameof(ImageUtility.Debayer), new Type[] { typeof(Bitmap), typeof(bool), typeof(bool), typeof(SensorType) })]
    class Patch_ImageUtility_DebayeredImageData {
        static bool Prefix(ref DebayeredImageData __result, Bitmap bmp, bool saveColorChannels = false, bool saveLumChannel = false, SensorType bayerPattern = SensorType.RGGB) {
            using (MyStopWatch.Measure()) {
                var filter = new BayerFilter16bppCPP();
                filter.SaveColorChannels = saveColorChannels;
                filter.SaveLumChannel = saveLumChannel;

                Logger.Debug($"Debayering pattern {bayerPattern}");

                switch (bayerPattern) {
                    case SensorType.RGGB:
                        filter.BayerPattern = new int[,] { { RGB.B, RGB.G }, { RGB.G, RGB.R } };
                        break;

                    case SensorType.RGBG:
                        filter.BayerPattern = new int[,] { { RGB.G, RGB.B }, { RGB.G, RGB.R } };
                        break;

                    case SensorType.GRGB:
                        filter.BayerPattern = new int[,] { { RGB.B, RGB.G }, { RGB.R, RGB.G } };
                        break;

                    case SensorType.GRBG:
                        filter.BayerPattern = new int[,] { { RGB.G, RGB.B }, { RGB.R, RGB.G } };
                        break;

                    case SensorType.GBGR:
                        filter.BayerPattern = new int[,] { { RGB.R, RGB.G }, { RGB.B, RGB.G } };
                        break;

                    case SensorType.GBRG:
                        filter.BayerPattern = new int[,] { { RGB.G, RGB.R }, { RGB.B, RGB.G } };
                        break;

                    case SensorType.BGRG:
                        filter.BayerPattern = new int[,] { { RGB.G, RGB.R }, { RGB.G, RGB.B } };
                        break;

                    case SensorType.BGGR:
                        filter.BayerPattern = new int[,] { { RGB.R, RGB.G }, { RGB.G, RGB.B } };
                        break;

                    default:
                        throw new InvalidImagePropertiesException(string.Format(Loc.Instance["LblUnsupportedCfaPattern"], bayerPattern));
                }

                DebayeredImageData debayered = new DebayeredImageData();
                debayered.ImageSource = ImageUtility.ConvertBitmap(filter.Apply(bmp), PixelFormats.Rgb48);
                debayered.ImageSource.Freeze();
                debayered.Data = filter.LRGBArrays;
                __result = debayered;
            }

            return false;
        }
    }

}
