#pragma once
#include "ninacl.hpp"



namespace LucasAlias::NINA::NinaPP::OpenCL {
	public ref struct DeviceInfo {
		System::String^ name;
		System::String^ vendor;

		DeviceInfo(const OpenCLDeviceInfo& di) {
			name = gcnew System::String(di.name.c_str());
			vendor = gcnew System::String(di.vendor.c_str());
		}
	};


	public ref class Manager {
	public:
		Manager() { _native = new OpenCLManager(); }
		!Manager() { 
			if (_native != nullptr) { 
				delete _native;
				_native = nullptr;
			} 
		}
		~Manager() { this->!Manager(); }



		void RefreshPlatformList() {
			try {
				_native->refreshPlatformList();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		System::UInt32 GetCLPlatformNumber() {
			try {
				return _native->getPlatformNumber();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}


		void RefreshDeviceList() {
			try {
				_native->refreshDeviceList();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}

		}
		System::UInt32 GetCLDeviceNumber() {
			try {
				return _native->getDeviceNumber();
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		void RefreshDeviceList(System::UInt32 platform) {
			try {
				_native->refreshDeviceList(platform);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		System::UInt32 GetCLDeviceNumber(System::UInt32 platform) {
			try {
				return _native->getDeviceNumber(platform);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}

		DeviceInfo^ GetDeviceInfo(System::UInt32 platform, System::UInt32 device) {
			try {
				return gcnew DeviceInfo(_native->getDeviceInfo(platform, device));
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}

		System::Collections::Generic::List<System::UInt32>^ CreateContext(System::Collections::Generic::List<System::Tuple<System::UInt32, System::UInt32>^>^ programs_devices) {
			auto d = std::vector<std::pair<size_t, size_t>>();
			for each (const auto %i in programs_devices) d.push_back(std::pair(i->Item1, i->Item2));

			try {
				auto c = _native->createContext(d);
				auto res = gcnew System::Collections::Generic::List<System::UInt32>(0);
				for (const auto& i : c) res->Add(i);
				return res;
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}
		System::UInt32 CreateContext(System::UInt32 program, System::UInt32 device) {
			try {
				return _native->createContext(program, device);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}


	private:
		OpenCLManager* _native = nullptr;
	};
}