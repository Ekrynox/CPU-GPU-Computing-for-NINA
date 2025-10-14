#pragma once
#include "CGPUNINA.hpp"

#include <algorithm>
#include <stdexcept>
#include <map>
#include <memory>
#include <string>
#include <vector>



namespace LucasAlias::NINA::CGPUNINA {

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

		size_t platformId = std::numeric_limits<size_t>::max();
		size_t deviceId = std::numeric_limits<size_t>::max();
	};
	typedef struct OpenCLDeviceInfo_ OpenCLDeviceInfo;



	class OpenCLManager {
	public:
		CGPUNINA_API OpenCLManager();
		CGPUNINA_API ~OpenCLManager();

		class Impl;
		Impl& GetImpl();

		CGPUNINA_API void refreshPlatformList();
		CGPUNINA_API size_t getPlatformNumber();

		CGPUNINA_API void refreshDeviceList();
		CGPUNINA_API size_t getDeviceNumber();
		CGPUNINA_API void refreshDeviceList(size_t platform);
		CGPUNINA_API size_t getDeviceNumber(size_t platform);

		CGPUNINA_API OpenCLDeviceInfo getDeviceInfo(size_t platform, size_t device);

		CGPUNINA_API std::vector<size_t> createContext(const std::vector<std::pair<size_t, size_t>>& platforms_devices);

		CGPUNINA_API size_t createExecutionContext(size_t platform, size_t device, const std::wstring& sourcePath, const std::vector<std::wstring>& sourceFiles);
		CGPUNINA_API void clearExecutionContextList();

	private:
		std::unique_ptr<Impl> _impl;
	};

}