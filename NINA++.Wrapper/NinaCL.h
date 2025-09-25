#pragma once
#include "ninacl.hpp"



namespace LucasAlias::NINA::NinaPP {
	public ref class CL {
	public:
		ref struct DeviceInfo {
			System::String^ name;
			System::String^ vendor;

			DeviceInfo(const OpenCLDeviceInfo& di) {
				name = gcnew System::String(di.name.c_str());
				vendor = gcnew System::String(di.vendor.c_str());
			}
		};


		static void refreshPlatformList() { OpenCLManager::Instance().refreshPlatformList(); };
		static System::UInt32 GetCLPlatformNumber() { return OpenCLManager::Instance().getPlatformNumber(); }

		static void refreshDeviceList() { OpenCLManager::Instance().refreshDeviceList(); };
		static System::UInt32 GetCLDeviceNumber() { return OpenCLManager::Instance().getDeviceNumber(); }
		static void refreshDeviceList(System::UInt32 platform) { OpenCLManager::Instance().refreshDeviceList(platform); };
		static System::UInt32 GetCLDeviceNumber(System::UInt32 platform) { return OpenCLManager::Instance().getDeviceNumber(platform); }

		static DeviceInfo^ getDeviceInfo(System::UInt32 platform, System::UInt32 device) { return gcnew DeviceInfo(OpenCLManager::Instance().getDeviceInfo(platform, device)); }

	};
}