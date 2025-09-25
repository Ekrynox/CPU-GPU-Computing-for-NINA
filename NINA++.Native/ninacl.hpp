#pragma once
#include "ninapp.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <vector>



namespace LucasAlias::NINA::NinaPP {

	class OpenCLManager {
	public:
		NINAPP_API static OpenCLManager& Instance();

		class Impl;
		Impl& GetImpl();

		NINAPP_API void refreshPlatformList();
		NINAPP_API size_t getPlatformsNumber();

		NINAPP_API void refreshDeviceList();
		NINAPP_API size_t getDevicesNumber();
		NINAPP_API void refreshDeviceList(size_t platform);
		NINAPP_API size_t getDevicesNumber(size_t platform);

	private:
		OpenCLManager();
		std::unique_ptr<Impl> _impl;
	};

}