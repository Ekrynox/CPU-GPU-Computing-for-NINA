#pragma once
#include "ninacl.hpp"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>



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

		OpenCLManager& GetNative() { return *_native; }


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


		System::UInt32 CreateExecutionContext(System::UInt32 platform, System::UInt32 device, System::String^ sourcePath, System::Collections::Generic::List<System::String^>^ sourceFiles) {
			auto sfs = std::vector<std::wstring>();
			for each (const auto sf in sourceFiles) {
				sfs.push_back(msclr::interop::marshal_as<std::wstring, System::String^>(sf));
			}
			try {
				return this->_native->createExecutionContext(platform, device, msclr::interop::marshal_as<std::wstring, System::String^>(sourcePath), sfs);
			}
			catch (const std::exception& e) {
				throw gcnew System::InvalidOperationException(gcnew System::String(e.what()));
			}
		}


	private:
		OpenCLManager* _native = nullptr;
	};
}