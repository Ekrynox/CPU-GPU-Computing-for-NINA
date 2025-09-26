using HarmonyLib;
using LucasAlias.NINA.NinaPP.Properties;
using NINA.Core.Model;
using NINA.Core.Utility;
using NINA.Image.ImageData;
using NINA.Plugin;
using NINA.Plugin.Interfaces;
using NINA.Core.Utility.Notification;
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
    public class NinaPP : PluginBase {
        private readonly IPluginOptionsAccessor pluginSettings;
        private readonly IProfileService profileService;

        private Harmony _harmony;
        private Object _harmonyLock = new Object();

        [ImportingConstructor]
        public NinaPP(IProfileService profileService, IOptionsVM options) {
            this.pluginSettings = new PluginOptionsAccessor(profileService, Guid.Parse(this.Identifier));
            this.profileService = profileService;

            NinaPPMediator.RegisterPlugin(this);

            this._harmony = new Harmony("com.example.patch");
            PatchAll();

            var opcl = new OpenCL.Manager();
            string devices = "";
            for (uint p = 0; p < opcl.GetCLPlatformNumber(); p++) {
                for (uint d = 0; d < opcl.GetCLDeviceNumber(p); d++) {
                    var info = opcl.getDeviceInfo(p, d);
                    devices += $"{p}:{d} -> {info.vendor} {info.name}\n";
                }
            }
            Notification.ShowSuccess($"Plateforms Number: {opcl.GetCLPlatformNumber()}\nDevices Number: {opcl.GetCLDeviceNumber()}\n{devices}");
        }

        public override Task Teardown() {
            UnPatchAll();
            return base.Teardown();
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void RaisePropertyChanged([CallerMemberName] string propertyName = null) {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            PatchAll();
        }

        private void ProfileService_ProfileChanged(object sender, EventArgs e) {}


        private void PatchAll() {
            lock (this._harmonyLock) {
                this._harmony.UnpatchAll(this._harmony.Id);

                if (NINA_Image_ImageAnalysis_BayerFilter16bpp) _harmony.PatchCategory("NINA_Image_ImageAnalysis_BayerFilter16bpp");
                if (NINA_Image_ImageAnalysis_ColorRemappingGeneral) _harmony.PatchCategory("NINA_Image_ImageAnalysis_ColorRemappingGeneral");
                if (NINA_Image_ImageAnalysis_FastGaussianBlur) _harmony.PatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur");
                if (NINA_Image_ImageAnalysis_StarDetection) _harmony.PatchCategory("NINA_Image_ImageAnalysis_StarDetection");


                if (Accord_Imaging_Filters_BinaryDilation3x3) _harmony.PatchCategory("Accord_Imaging_Filters_BinaryDilation3x3");
                if (Accord_Imaging_Filters_CannyEdgeDetector) _harmony.PatchCategory("Accord_Imaging_Filters_CannyEdgeDetector");
                if (Accord_Imaging_Filters_Convolution) _harmony.PatchCategory("Accord_Imaging_Filters_Convolution");
                if (Accord_Imaging_Filters_NoBlurCannyEdgeDetector) _harmony.PatchCategory("Accord_Imaging_Filters_NoBlurCannyEdgeDetector");
                if (Accord_Imaging_Filters_ResizeBicubic) _harmony.PatchCategory("Accord_Imaging_Filters_ResizeBicubic");
                if (Accord_Imaging_Filters_SISThreshold) _harmony.PatchCategory("Accord_Imaging_Filters_SISThreshold");

                if (Accord_Imaging_BlobCounter) _harmony.PatchCategory("Accord_Imaging_BlobCounter");
                if (Accord_Imaging_BlobCounterBase) _harmony.PatchCategory("Accord_Imaging_BlobCounterBase");

                this._harmony.PatchAllUncategorized();
            }
        }

        private void UnPatchAll() {
            lock (this._harmonyLock) {
                this._harmony.UnpatchAll(this._harmony.Id);
            }
        }


        public bool NINA_Image_ImageAnalysis_BayerFilter16bpp {
            get => Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp = value;
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public bool NINA_Image_ImageAnalysis_BayerFilter16bpp__MT {
            get => Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT;
            set {
                Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp__MT = value;
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
        public bool Accord_Imaging_Filters_Convolution__MT {
            get => Settings.Default.Accord_Imaging_Filters_Convolution__MT;
            set {
                Settings.Default.Accord_Imaging_Filters_Convolution__MT = value;
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
        public bool Accord_Imaging_Filters_ResizeBicubic__MT {
            get => Settings.Default.Accord_Imaging_Filters_ResizeBicubic__MT;
            set {
                Settings.Default.Accord_Imaging_Filters_ResizeBicubic__MT = value;
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
