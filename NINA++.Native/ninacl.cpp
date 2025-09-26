#include "ninacl_internal.hpp"

#include <fstream>



namespace LucasAlias::NINA::NinaPP {

	OpenCLManager& OpenCLManager::Instance() {
		static OpenCLManager instance = OpenCLManager();
		return instance;
	}
	OpenCLManager::OpenCLManager() : _impl(std::make_unique<Impl>()) {
		refreshPlatformList();
	}
	OpenCLManager::Impl& OpenCLManager::GetImpl() { return *(this->_impl); }


	std::vector<cl::Platform> OpenCLManager::Impl::getPlatformList() {
		auto platforms = std::vector<cl::Platform>();
		cl::Platform::get(&platforms);
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


	std::vector<cl::Device> OpenCLManager::Impl::getDeviceList(const cl::Platform &platform) {
		auto devices = std::vector<cl::Device>();
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		return devices;
	}
	size_t OpenCLManager::Impl::getDeviceNumber(const cl::Platform &platform) {
		return getDeviceList(platform).size();
	}

	void OpenCLManager::refreshDeviceList() {
		this->_impl->devices.clear();
		for (const auto &p : this->_impl->platforms) {
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


	OpenCLDeviceInfo OpenCLManager::Impl::getDeviceInfo(const cl::Device &device) {
		auto deviceInfo = OpenCLDeviceInfo();

		deviceInfo.name = device.getInfo<CL_DEVICE_NAME>();
		deviceInfo.vendor = device.getInfo<CL_DEVICE_VENDOR>();

		return deviceInfo;
	}

	OpenCLDeviceInfo OpenCLManager::getDeviceInfo(size_t platform, size_t device) {
		if (platform >= this->_impl->platforms.size()) throw OpenCLPlatformNotFound();
		auto p = this->_impl->platforms[platform];
		if (device >= this->_impl->devices[p].size()) throw OpenCLDeviceNotFound();
		auto d = this->_impl->devices[p][device];
		return this->_impl->getDeviceInfo(d);
	}


	cl::Context OpenCLManager::Impl::createContext(std::vector<cl::Device>& devices) {
		cl_int err = CL_SUCCESS;
		auto context = cl::Context(devices, nullptr, nullptr, nullptr, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while creating the OpenCL context!");
		return context;
	}

	cl::CommandQueue OpenCLManager::Impl::createCommandQueue(cl::Context &context, cl::Device &device) {
		cl_int err = CL_SUCCESS;
		auto command = cl::CommandQueue(context, device, 0, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while creating the OpenCL command queue!");
		return command;
	}

	cl::Program OpenCLManager::Impl::buildProgram(cl::Context &context, const std::vector<std::wstring> &sourceFiles) {
		auto sources = cl::Program::Sources();
		for (auto sf : sourceFiles) {
			auto sourceStream = std::ifstream(sf);
			auto sourceCode = std::string(std::istreambuf_iterator<char>(sourceStream), (std::istreambuf_iterator<char>()));
			sources.push_back(sourceCode);
		}

		cl_int err = CL_SUCCESS;
		auto program = cl::Program(context, sources, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while building the OpenCL program!");
		return program;
	}
}