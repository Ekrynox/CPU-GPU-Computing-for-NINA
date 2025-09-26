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


		static void refreshPlatformList() {
			try {
				OpenCLManager::Instance().refreshPlatformList();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		static System::UInt32 GetCLPlatformNumber() {
			try {
				return OpenCLManager::Instance().getPlatformNumber();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}


		static void refreshDeviceList() {
			try {
				OpenCLManager::Instance().refreshDeviceList();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}

		}
		static System::UInt32 GetCLDeviceNumber() {
			try {
				return OpenCLManager::Instance().getDeviceNumber();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		static void refreshDeviceList(System::UInt32 platform) {
			try {
				OpenCLManager::Instance().refreshDeviceList(platform);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		static System::UInt32 GetCLDeviceNumber(System::UInt32 platform) {
			try {
				return OpenCLManager::Instance().getDeviceNumber(platform);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}

		static DeviceInfo^ getDeviceInfo(System::UInt32 platform, System::UInt32 device) {
			try {
				return gcnew DeviceInfo(OpenCLManager::Instance().getDeviceInfo(platform, device));
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}

	};
}