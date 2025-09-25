#include "ninacl_internal.hpp"



namespace LucasAlias::NINA::NinaPP {

	OpenCLManager& OpenCLManager::Instance() {
		static OpenCLManager instance = OpenCLManager();
		return instance;
	}
	OpenCLManager::OpenCLManager() : _impl(std::make_unique<Impl>()) {
		refreshPlatformList();
	}
	OpenCLManager::Impl& OpenCLManager::GetImpl() { return *(this->_impl); }


	std::vector<cl_platform_id> OpenCLManager::Impl::getPlatformList() {
		cl_uint num_platforms;
		clGetPlatformIDs(0, nullptr, &num_platforms);
		auto platforms = std::vector<cl_platform_id>(num_platforms);
		clGetPlatformIDs(num_platforms, platforms.data(), nullptr);
		return platforms;
	}
	size_t OpenCLManager::Impl::getPlatformNumber() {
		return getPlatformList().size();
	}

	void OpenCLManager::refreshPlatformList() {
		this->_impl->platforms = this->_impl->getPlatformList();
		refreshDeviceList();
	}
	size_t OpenCLManager::getPlatformNumber() {
		return this->_impl->platforms.size();
	}


	std::vector<cl_device_id> OpenCLManager::Impl::getDeviceList(cl_platform_id platform) {
		cl_uint num_devices;
		if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices) == CL_INVALID_PLATFORM) throw OpenCLPlatformNotExisting();
		auto devices = std::vector<cl_device_id>(num_devices);
		if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, devices.data(), nullptr) == CL_INVALID_PLATFORM) throw OpenCLPlatformNotExisting();
		return devices;
	}
	size_t OpenCLManager::Impl::getDeviceNumber(cl_platform_id platform) {
		return getDeviceList(platform).size();
	}

	void OpenCLManager::refreshDeviceList() {
		this->_impl->devices.clear();
		for (auto p : this->_impl->platforms) {
			this->_impl->devices[p] = this->_impl->getDeviceList(p);
		}
	}
	size_t OpenCLManager::getDeviceNumber() {
		return this->_impl->devices.size();
	}
	void OpenCLManager::refreshDeviceList(size_t platform) {
		if (platform >= this->_impl->platforms.size()) throw OpenCLPlatformNotFound();
		auto p = this->_impl->platforms[platform];
		this->_impl->devices.erase(p);
		this->_impl->devices[p] = this->_impl->getDeviceList(p);
	}
	size_t OpenCLManager::getDeviceNumber(size_t platform) {
		if (platform >= this->_impl->platforms.size()) throw OpenCLPlatformNotFound();
		return this->_impl->devices[this->_impl->platforms[platform]].size();
	}


	OpenCLDeviceInfo OpenCLManager::Impl::getDeviceInfo(cl_platform_id platform, cl_device_id device) {
		char buffer[128];
		auto deviceInfo = OpenCLDeviceInfo();

		if (clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(buffer), buffer, NULL) == CL_INVALID_DEVICE) throw OpenCLDeviceNotExisting();
		deviceInfo.name = std::string(buffer, sizeof(buffer));
		if (clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL) == CL_INVALID_DEVICE) throw OpenCLDeviceNotExisting();
		deviceInfo.vendor = std::string(buffer, sizeof(buffer));

		return deviceInfo;
	}

	OpenCLDeviceInfo OpenCLManager::getDeviceInfo(size_t platform, size_t device) {
		if (platform >= this->_impl->platforms.size()) throw OpenCLPlatformNotFound();
		auto p = this->_impl->platforms[platform];
		if (device >= this->_impl->devices[p].size()) throw OpenCLDeviceNotFound();
		auto d = this->_impl->devices[p][device];

		return this->_impl->getDeviceInfo(p, d);
	}

}