using HarmonyLib;
using LucasAlias.NINA.NinaPP.Properties;
using NINA.Core.Model;
using NINA.Core.Utility;
using NINA.Image.ImageData;
using NINA.Plugin;
using NINA.Plugin.Interfaces;
using NINA.Profile;
using NINA.Profile.Interfaces;
using NINA.WPF.Base.Interfaces.Mediator;
using NINA.WPF.Base.Interfaces.ViewModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Settings = LucasAlias.NINA.NinaPP.Properties.Settings;

namespace LucasAlias.NINA.NinaPP {
    [Export(typeof(IPluginManifest))]
    public class Nina : PluginBase {
        private readonly IPluginOptionsAccessor pluginSettings;
        private readonly IProfileService profileService;

        [ImportingConstructor]
        public Nina(IProfileService profileService, IOptionsVM options) {
            this.pluginSettings = new PluginOptionsAccessor(profileService, Guid.Parse(this.Identifier));
            this.profileService = profileService;

            var harmony = new Harmony("com.example.patch");

            if (NINA_Image_ImageAnalysis_BayerFilter16bpp) harmony.PatchCategory("NINA_Image_ImageAnalysis_BayerFilter16bpp");
            if (NINA_Image_ImageAnalysis_ColorRemappingGeneral) harmony.PatchCategory("NINA_Image_ImageAnalysis_ColorRemappingGeneral");
            if (NINA_Image_ImageAnalysis_FastGaussianBlur) harmony.PatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur");
            if (NINA_Image_ImageAnalysis_StarDetection) harmony.PatchCategory("NINA_Image_ImageAnalysis_StarDetection");


            if (Accord_Imaging_Filters_BinaryDilation3x3) harmony.PatchCategory("Accord_Imaging_Filters_BinaryDilation3x3");
            if (Accord_Imaging_Filters_CannyEdgeDetector) harmony.PatchCategory("Accord_Imaging_Filters_CannyEdgeDetector");
            if (Accord_Imaging_Filters_Convolution) harmony.PatchCategory("Accord_Imaging_Filters_Convolution");
            if (Accord_Imaging_Filters_NoBlurCannyEdgeDetector) harmony.PatchCategory("Accord_Imaging_Filters_NoBlurCannyEdgeDetector");
            if (Accord_Imaging_Filters_ResizeBicubic) harmony.PatchCategory("Accord_Imaging_Filters_ResizeBicubic");
            if (Accord_Imaging_Filters_SISThreshold) harmony.PatchCategory("Accord_Imaging_Filters_SISThreshold");

            if (Accord_Imaging_BlobCounter) harmony.PatchCategory("Accord_Imaging_BlobCounter");
            if (Accord_Imaging_BlobCounterBase) harmony.PatchCategory("Accord_Imaging_BlobCounterBase");

            harmony.PatchAllUncategorized();
        }

        public override Task Teardown() {
            return base.Teardown();
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void RaisePropertyChanged([CallerMemberName] string propertyName = null) {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void ProfileService_ProfileChanged(object sender, EventArgs e) {}



        public bool NINA_Image_ImageAnalysis_BayerFilter16bpp {
            get => Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool NINA_Image_ImageAnalysis_ColorRemappingGeneral {
            get => Settings.Default.NINA_Image_ImageAnalysis_ColorRemappingGeneral;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_ColorRemappingGeneral = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool NINA_Image_ImageAnalysis_FastGaussianBlur {
            get => Settings.Default.NINA_Image_ImageAnalysis_FastGaussianBlur;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_FastGaussianBlur = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool NINA_Image_ImageAnalysis_StarDetection {
            get => Settings.Default.NINA_Image_ImageAnalysis_StarDetection;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_StarDetection = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }


        public bool Accord_Imaging_Filters_BinaryDilation3x3 {
            get => Settings.Default.Accord_Imaging_Filters_BinaryDilation3x3;
            set {
                Settings.Default.Accord_Imaging_Filters_BinaryDilation3x3 = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_Filters_CannyEdgeDetector {
            get => Settings.Default.Accord_Imaging_Filters_CannyEdgeDetector;
            set {
                Settings.Default.Accord_Imaging_Filters_CannyEdgeDetector = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_Filters_Convolution {
            get => Settings.Default.Accord_Imaging_Filters_Convolution;
            set {
                Settings.Default.Accord_Imaging_Filters_Convolution = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_Filters_NoBlurCannyEdgeDetector {
            get => Settings.Default.Accord_Imaging_Filters_NoBlurCannyEdgeDetector;
            set {
                Settings.Default.Accord_Imaging_Filters_NoBlurCannyEdgeDetector = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_Filters_ResizeBicubic {
            get => Settings.Default.Accord_Imaging_Filters_ResizeBicubic;
            set {
                Settings.Default.Accord_Imaging_Filters_ResizeBicubic = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_Filters_SISThreshold {
            get => Settings.Default.Accord_Imaging_Filters_SISThreshold;
            set {
                Settings.Default.Accord_Imaging_Filters_SISThreshold = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }

        public bool Accord_Imaging_BlobCounter {
            get => Settings.Default.Accord_Imaging_BlobCounter;
            set {
                Settings.Default.Accord_Imaging_BlobCounter = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool Accord_Imaging_BlobCounterBase {
            get => Settings.Default.Accord_Imaging_BlobCounterBase;
            set {
                Settings.Default.Accord_Imaging_BlobCounterBase = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }

    }
}
