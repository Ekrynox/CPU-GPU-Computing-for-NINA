#include "ninacl_internal.hpp"



namespace LucasAlias::NINA::NinaPP {

	OpenCLManager& OpenCLManager::Instance() {
		static OpenCLManager instance = OpenCLManager();
		return instance;
	}

	OpenCLManager::OpenCLManager() : _impl(std::make_unique<Impl>()) {
		refreshPlatformList();
		refreshDeviceList();
	}

	OpenCLManager::Impl& OpenCLManager::GetImpl() { return *(this->_impl); }


	void OpenCLManager::refreshPlatformList() {
		cl_uint num_platforms;
		clGetPlatformIDs(0, nullptr, &num_platforms);
		this->_impl->platforms = std::vector<cl_platform_id>(num_platforms);
		clGetPlatformIDs(num_platforms, this->_impl->platforms.data(), nullptr);

		// Remove device list for not found plateforms
		std::erase_if(this->_impl->devices, [&](auto const& dl) { return std::find(this->_impl->platforms.begin(), this->_impl->platforms.end(), dl.first) == this->_impl->platforms.end(); });
	}

	size_t OpenCLManager::getPlatformNumber() {
		return this->_impl->platforms.size();
	}


	void OpenCLManager::Impl::refreshDeviceList(cl_platform_id platform) {
		this->devices.erase(platform);

		cl_uint num_devices;
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
		auto devices = &(this->devices[platform] = std::vector<cl_device_id>(num_devices));
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, devices->data(), nullptr);
	}

	size_t  OpenCLManager::Impl::getDeviceNumber(cl_platform_id platform) {
		if (this->devices.contains(platform)) return this->devices[platform].size();
		return 0;
	}

	void OpenCLManager::refreshDeviceList() {
		this->_impl->devices.clear();
		for (auto p : this->_impl->platforms) {
			this->_impl->refreshDeviceList(p);
		}
	}

	size_t OpenCLManager::getDeviceNumber() {
		size_t nb = 0;
		for (auto p : this->_impl->platforms) {
			nb += this->_impl->getDeviceNumber(p);
		}
		return nb;
	}

	void OpenCLManager::refreshDeviceList(size_t platform) {
		if (platform >= this->_impl->platforms.size()) return;
		this->_impl->refreshDeviceList(this->_impl->platforms[platform]);
	}

	size_t OpenCLManager::getDeviceNumber(size_t platform) {
		if (platform >= this->_impl->platforms.size()) return 0;
		return this->_impl->getDeviceNumber(this->_impl->platforms[platform]);
	}

}