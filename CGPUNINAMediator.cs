using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LucasAlias.NINA.CGPUNINA {
    public class CGPUNINAMediator {
        public static CGPUNINA Plugin { get; private set; }
        public static OpenCL.Manager OpenCLManager { get; private set; }


        internal static void RegisterPlugin(CGPUNINA plugin) {
            Plugin = plugin;
        }

        internal static void RegisterOpenCLManager(OpenCL.Manager openCLManager) {
            OpenCLManager = openCLManager;
        }
    }
}
