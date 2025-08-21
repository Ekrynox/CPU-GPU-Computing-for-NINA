using Accord.Imaging;
using NINA.Core.Enum;
using NINA.Core.Locale;
using NINA.Core.Utility;
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

    public class ImageUtility {

        public static DebayeredImageData Debayer(Bitmap bmp, bool saveColorChannels = false, bool saveLumChannel = false, SensorType bayerPattern = SensorType.RGGB) {
            using (MyStopWatch.Measure()) {
                return new DebayeredImageData();
            }

        }
    }

}
