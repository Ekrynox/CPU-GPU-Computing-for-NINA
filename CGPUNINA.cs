using HarmonyLib;
using LucasAlias.NINA.CGPUNINA.Properties;
using Newtonsoft.Json.Linq;
using NINA.Core.Model;
using NINA.Core.Utility;
using NINA.Core.Utility.Notification;
using NINA.Image.ImageData;
using NINA.Plugin;
using NINA.Plugin.Interfaces;
using NINA.Profile;
using NINA.Profile.Interfaces;
using NINA.WPF.Base.Interfaces.Mediator;
using NINA.WPF.Base.Interfaces.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Settings = LucasAlias.NINA.CGPUNINA.Properties.Settings;

namespace LucasAlias.NINA.CGPUNINA {
    [Export(typeof(IPluginManifest))]
    public class CGPUNINA : PluginBase {
        private readonly IPluginOptionsAccessor pluginSettings;
        private readonly IProfileService profileService;

        private Harmony _harmony;
        private Object _harmonyLock = new Object();

        [ImportingConstructor]
        public CGPUNINA(IProfileService profileService, IOptionsVM options) {
            this.pluginSettings = new PluginOptionsAccessor(profileService, Guid.Parse(this.Identifier));
            this.profileService = profileService;

            CGPUNINAMediator.RegisterPlugin(this);
            CGPUNINAMediator.RegisterOpenCLManager(new OpenCL.Manager());

            OpenCLAvailableGpus.Add(new OpenCL.DeviceInfo());
            for (uint p = 0; p < CGPUNINAMediator.OpenCLManager.GetCLPlatformNumber(); p++) {
                for (uint d = 0; d < CGPUNINAMediator.OpenCLManager.GetCLDeviceNumber(p); d++) {
                    OpenCLAvailableGpus.Add(CGPUNINAMediator.OpenCLManager.GetDeviceInfo(p, d));
                }
            }

            this._harmony = new Harmony("com.example.patch");
            PatchAll();
        }

        public override Task Teardown() {
            lock (this._ctsLock) {
                _cts?.Cancel();
            }
            UnPatchAll();
            return base.Teardown();
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void RaisePropertyChanged([CallerMemberName] string propertyName = null) {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            PatchAll();
        }

        private void ProfileService_ProfileChanged(object sender, EventArgs e) {}


        public ObservableCollection<OpenCL.DeviceInfo> OpenCLAvailableGpus { get; } = new ObservableCollection<OpenCL.DeviceInfo>();

        private readonly object _ctsLock = new object();
        private CancellationTokenSource _cts;
        private readonly TimeSpan _delay = TimeSpan.FromSeconds(5);


        private void PatchAll() {
            CancellationToken token;
            lock (_ctsLock) {
                _cts?.Cancel();
                _cts = new CancellationTokenSource();
                token = _cts.Token;
            }

            _ = PatchAllTask(token);
        }
        private async Task PatchAllTask(CancellationToken token) {
            Notification.ShowInformation("CPU\\GPU Computing: Settings will be applied in 5 seconds.", _delay);
            await Task.Delay(_delay, token);
            if (token.IsCancellationRequested) return;

            try {
                this.UnPatchAll();

                lock (this._harmonyLock) {
                    if (NINA_Image_ImageAnalysis_BayerFilter16bpp) {
                        _harmony.PatchCategory("NINA_Image_ImageAnalysis_BayerFilter16bpp");
                        var info = NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL;
                        if (info != null) NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL_Context = CGPUNINAMediator.OpenCLManager.CreateExecutionContext(info.PlatformId, info.DeviceId, Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), new List<string>(["BayerFilter16bpp.cl"]));
                    }
                    if (NINA_Image_ImageAnalysis_ColorRemappingGeneral) _harmony.PatchCategory("NINA_Image_ImageAnalysis_ColorRemappingGeneral");
                    if (NINA_Image_ImageAnalysis_FastGaussianBlur) _harmony.PatchCategory("NINA_Image_ImageAnalysis_FastGaussianBlur");
                    if (NINA_Image_ImageAnalysis_StarDetection) _harmony.PatchCategory("NINA_Image_ImageAnalysis_StarDetection");


                    if (Accord_Imaging_Filters_BinaryDilation3x3) _harmony.PatchCategory("Accord_Imaging_Filters_BinaryDilation3x3");
                    if (Accord_Imaging_Filters_CannyEdgeDetector) _harmony.PatchCategory("Accord_Imaging_Filters_CannyEdgeDetector");
                    if (Accord_Imaging_Filters_Convolution) {
                        _harmony.PatchCategory("Accord_Imaging_Filters_Convolution");
                        var info = Accord_Imaging_Filters_Convolution__OpCL;
                        if (info != null) Accord_Imaging_Filters_Convolution__OpCL_Context = CGPUNINAMediator.OpenCLManager.CreateExecutionContext(info.PlatformId, info.DeviceId, Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), new List<string>(["Convolution.cl"]));
                    }
                    if (Accord_Imaging_Filters_NoBlurCannyEdgeDetector) _harmony.PatchCategory("Accord_Imaging_Filters_NoBlurCannyEdgeDetector");
                    if (Accord_Imaging_Filters_ResizeBicubic) {
                        _harmony.PatchCategory("Accord_Imaging_Filters_ResizeBicubic");
                        var info = Accord_Imaging_Filters_ResizeBicubic__OpCL;
                        if (info != null) Accord_Imaging_Filters_ResizeBicubic__OpCL_Context = CGPUNINAMediator.OpenCLManager.CreateExecutionContext(info.PlatformId, info.DeviceId, Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), new List<string>(["ResizeBicubic.cl"]));
                    }
                    if (Accord_Imaging_Filters_SISThreshold) _harmony.PatchCategory("Accord_Imaging_Filters_SISThreshold");

                    if (Accord_Imaging_BlobCounter) _harmony.PatchCategory("Accord_Imaging_BlobCounter");
                    if (Accord_Imaging_BlobCounterBase) _harmony.PatchCategory("Accord_Imaging_BlobCounterBase");

                    this._harmony.PatchAllUncategorized();
                }

                Notification.ShowSuccess("CPU\\GPU Computing: Settings have been applied.");
            } 
            catch (OperationCanceledException) { } 
            catch (Exception ex) {
                Notification.ShowError("CPU\\GPU Computing: Failed to apply settings !");
                Logger.Error(ex);
            }
        }
        private void UnPatchAll() {
            lock (this._harmonyLock) {
                this._harmony.UnpatchAll(this._harmony.Id);

                CGPUNINAMediator.OpenCLManager.ClearExecutionContextList();
                NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL_Context = null;
                Accord_Imaging_Filters_ResizeBicubic__OpCL_Context = null;
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
        public OpenCL.DeviceInfo? NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL {
            get {
                var i = OpenCLAvailableGpus.Where(e => $"{e.Vendor} -> {e.Name}" == Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL);
                return i.Count() > 0 ? (i.First().Name == "" && i.First().Vendor == "" ? null : i.First()) : null;
            }
            set {
                Settings.Default.NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL = (value != null) ? $"{value.Vendor} -> {value.Name}" : "";
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public uint? NINA_Image_ImageAnalysis_BayerFilter16bpp__OpCL_Context = null;

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
        public OpenCL.DeviceInfo? Accord_Imaging_Filters_Convolution__OpCL {
            get {
                var i = OpenCLAvailableGpus.Where(e => $"{e.Vendor} -> {e.Name}" == Settings.Default.Accord_Imaging_Filters_Convolution__OpCL);
                return i.Count() > 0 ? (i.First().Name == "" && i.First().Vendor == "" ? null : i.First()) : null;
            }
            set {
                Settings.Default.Accord_Imaging_Filters_Convolution__OpCL = (value != null) ? $"{value.Vendor} -> {value.Name}" : "";
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public uint? Accord_Imaging_Filters_Convolution__OpCL_Context = null;

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
        public OpenCL.DeviceInfo? Accord_Imaging_Filters_ResizeBicubic__OpCL {
            get {
                var i = OpenCLAvailableGpus.Where(e => $"{e.Vendor} -> {e.Name}" == Settings.Default.Accord_Imaging_Filters_ResizeBicubic__OpCL);
                return i.Count() > 0 ? (i.First().Name == "" && i.First().Vendor == "" ? null : i.First()) : null;
            }
            set {
                Settings.Default.Accord_Imaging_Filters_ResizeBicubic__OpCL = (value != null) ? $"{value.Vendor} -> {value.Name}" : "";
                CoreUtil.SaveSettings(Settings.Default);
                RaisePropertyChanged();
            }
        }
        public uint? Accord_Imaging_Filters_ResizeBicubic__OpCL_Context = null;

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
