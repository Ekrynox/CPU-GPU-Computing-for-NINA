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
	class OpenCLContextNotFound : public std::runtime_error {
	public:
		explicit OpenCLContextNotFound() : std::runtime_error("OpenCLContextNotFound: The provided id was not found in the list!") {};
	};


	struct OpenCLDeviceInfo_ {
		std::string name = "";
		std::string vendor = "";
	};
	typedef struct OpenCLDeviceInfo_ OpenCLDeviceInfo;



	class OpenCLManager {
	public:
		NINAPP_API OpenCLManager();
		NINAPP_API ~OpenCLManager();

		class Impl;
		Impl& GetImpl();

		NINAPP_API void refreshPlatformList();
		NINAPP_API size_t getPlatformNumber();

		NINAPP_API void refreshDeviceList();
		NINAPP_API size_t getDeviceNumber();
		NINAPP_API void refreshDeviceList(size_t platform);
		NINAPP_API size_t getDeviceNumber(size_t platform);

		NINAPP_API OpenCLDeviceInfo getDeviceInfo(size_t platform, size_t device);

		NINAPP_API std::vector<size_t> createContext(const std::vector<std::pair<size_t, size_t>>& platforms_devices);
		NINAPP_API size_t createContext(size_t platform, size_t device);

		NINAPP_API size_t createCommandQueue(size_t platform, size_t device, size_t context);

		NINAPP_API size_t buildProgram(size_t platform, size_t device, size_t context, const std::wstring &sourceFile);

	private:
		std::unique_ptr<Impl> _impl;
	};

}