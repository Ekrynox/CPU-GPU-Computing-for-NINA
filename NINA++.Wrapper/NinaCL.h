#pragma once
#include "ninacl.hpp"



namespace LucasAlias::NINA::NinaPP {
	public ref class CL {
	public:

		static System::UInt32 GetCLPlatformNumber() {
			return OpenCLManager::Instance().getPlatformNumber();
		}

		static System::UInt32 GetCLDeviceNumber() {
			return OpenCLManager::Instance().getDeviceNumber();
		}

	};
}