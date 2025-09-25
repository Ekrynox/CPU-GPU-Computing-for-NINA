#pragma once
#include "ninacl.hpp"



namespace LucasAlias::NINA::NinaPP {
	public ref class CL {
	public:

		static System::UInt32 GetCLPlatformsNumber() {
			return OpenCLManager::Instance().getPlatformsNumber();
		}

		static System::UInt32 GetCLDevicesNumber() {
			return OpenCLManager::Instance().getDevicesNumber();
		}

	};
}