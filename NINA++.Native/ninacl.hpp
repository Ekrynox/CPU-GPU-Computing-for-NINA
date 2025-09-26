#pragma once
#include "ninapp.hpp"

#include <algorithm>
#include <stdexcept>
#include <map>
#include <memory>
#include <string>
#include <vector>



namespace LucasAlias::NINA::NinaPP {

	class OpenCLPlatformNotFound : public std::runtime_error {
	public:
		explicit OpenCLPlatformNotFound() : std::runtime_error("OpenCLPlatformNotFound: The provided id was not found in the list!") {};
	};
	class OpenCLDeviceNotFound : public std::runtime_error {
	public:
		explicit OpenCLDeviceNotFound() : std::runtime_error("OpenCLDeviceNotFound: The provided id was not found in the list!") {};
	};


	struct OpenCLDeviceInfo_ {
		std::string name = "";
		std::string vendor = "";
	};
	typedef struct OpenCLDeviceInfo_ OpenCLDeviceInfo;



	class OpenCLManager {
	public:
		NINAPP_API static OpenCLManager& Instance();

		class Impl;
		Impl& GetImpl();

		NINAPP_API void refreshPlatformList();
		NINAPP_API size_t getPlatformNumber();

		NINAPP_API void refreshDeviceList();
		NINAPP_API size_t getDeviceNumber();
		NINAPP_API void refreshDeviceList(size_t platform);
		NINAPP_API size_t getDeviceNumber(size_t platform);

		NINAPP_API OpenCLDeviceInfo getDeviceInfo(size_t platform, size_t device);

	private:
		OpenCLManager();
		std::unique_ptr<Impl> _impl;
	};

}