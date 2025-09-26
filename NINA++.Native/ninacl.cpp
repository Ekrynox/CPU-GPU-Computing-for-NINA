#include "ninacl_internal.hpp"

#include <fstream>



namespace LucasAlias::NINA::NinaPP {

	OpenCLManager::OpenCLManager() : _impl(std::make_unique<Impl>()) {
		refreshPlatformList();
	}
	OpenCLManager::~OpenCLManager() {
		//this->_impl->programs.clear();
		this->_impl->commandQ.clear();
		this->_impl->contexts.clear();

		this->_impl->devices.clear();
		this->_impl->platforms.clear();
	}
	OpenCLManager::Impl& OpenCLManager::GetImpl() { return *(this->_impl); }


	cl::Platform& OpenCLManager::Impl::getPlatform(size_t platform) {
		if (platform >= this->platforms.size()) throw OpenCLPlatformNotFound();
		return this->platforms[platform];
	}
	cl::Device& OpenCLManager::Impl::getDevice(const cl::Platform& platform, size_t device) {
		if (device >= this->devices[platform].size()) throw OpenCLDeviceNotFound();
		return this->devices[platform][device];
	}
	cl::Device& OpenCLManager::Impl::getDevice(size_t platform, size_t device) {
		return getDevice(getPlatform(platform), device);
	}


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
		size_t nb = 0;
		for (const auto& i : this->_impl->devices) {
			nb += i.second.size();
		}
		return nb;
	}
	void OpenCLManager::refreshDeviceList(size_t platform) {
		auto& p = this->_impl->getPlatform(platform);
		this->_impl->devices.erase(p);
		this->_impl->devices[p] = this->_impl->getDeviceList(p);
	}
	size_t OpenCLManager::getDeviceNumber(size_t platform) {
		return this->_impl->devices[this->_impl->getPlatform(platform)].size();
	}


	OpenCLDeviceInfo OpenCLManager::Impl::getDeviceInfo(const cl::Device &device) {
		auto deviceInfo = OpenCLDeviceInfo();

		deviceInfo.name = device.getInfo<CL_DEVICE_NAME>();
		deviceInfo.vendor = device.getInfo<CL_DEVICE_VENDOR>();

		return deviceInfo;
	}

	OpenCLDeviceInfo OpenCLManager::getDeviceInfo(size_t platform, size_t device) {
		auto& p = this->_impl->getPlatform(platform);
		if (device >= this->_impl->devices[p].size()) throw OpenCLDeviceNotFound();
		auto& d = this->_impl->devices[p][device];
		return this->_impl->getDeviceInfo(this->_impl->getDevice(platform, device));
	}



	cl::Context& OpenCLManager::Impl::getContext(const cl::Device& device, size_t context) {
		if (!this->contexts.contains(device)) throw OpenCLContextNotFound();
		if (context >= this->contexts[device].size()) throw OpenCLContextNotFound();
		return this->contexts[device][context];
	}
	cl::Context& OpenCLManager::Impl::getContext(const cl::Platform& platform, size_t device, size_t context) {
		return this->getContext(this->getDevice(platform, device), context);
	}
	cl::Context& OpenCLManager::Impl::getContext(size_t platform, size_t device, size_t context) {
		return this->getContext(this->getDevice(platform, device), context);
	}


	cl::Context OpenCLManager::Impl::createContext(const std::vector<cl::Device>& devices) {
		cl_int err = CL_SUCCESS;
		auto context = cl::Context(devices, nullptr, nullptr, nullptr, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while creating the OpenCL context!");
		return context;
	}
	cl::CommandQueue OpenCLManager::Impl::createCommandQueue(const cl::Device& device, const cl::Context &context) {
		cl_int err = CL_SUCCESS;
		auto command = cl::CommandQueue(context, device, 0, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while creating the OpenCL command queue!");
		return command;
	}
	cl::Program OpenCLManager::Impl::buildProgram(const cl::Context &context, const std::wstring &sourceFile) {
		auto sourceStream = std::ifstream(sourceFile);
		auto sourceCode = std::string(std::istreambuf_iterator<char>(sourceStream), (std::istreambuf_iterator<char>()));

		cl_int err = CL_SUCCESS;
		auto program = cl::Program(context, sourceCode, true, &err);
		if (err != CL_SUCCESS) throw std::runtime_error("Error while building the OpenCL program!");
		return program;
	}

	std::vector<size_t> OpenCLManager::createContext(const std::vector<std::pair<size_t, size_t>>& platforms_devices) {
		auto d = std::vector<cl::Device>();
		for (const auto& i : platforms_devices) {
			d.push_back(this->_impl->getDevice(i.first, i.second));
		}

		auto c = this->_impl->createContext(d);
		auto cl = std::vector<size_t>();
		for (const auto& i : d) {
			if (!this->_impl->contexts.contains(i)) this->_impl->contexts[i] = std::vector<cl::Context>();
			this->_impl->contexts[i].push_back(c);
			cl.push_back(this->_impl->contexts[i].size() - 1);
		}

		return cl;
	}
	size_t OpenCLManager::createContext(size_t platform, size_t device) {
		return createContext(std::vector({ std::pair(platform, device) }))[0];
	}

	size_t OpenCLManager::createCommandQueue(size_t platform, size_t device, size_t context) {
		auto& d = this->_impl->getDevice(platform, device);
		auto& c = this->_impl->getContext(d, context);
		auto q = this->_impl->createCommandQueue(d, c);
		
		if (!this->_impl->commandQ.contains(std::pair(d, c))) this->_impl->commandQ[std::pair(d, c)] = std::vector<cl::CommandQueue>();
		this->_impl->commandQ[std::pair(d, c)].push_back(q);
		return this->_impl->commandQ[std::pair(d, c)].size() - 1;
	}

	size_t OpenCLManager::buildProgram(size_t platform, size_t device, size_t context, const std::wstring &sourceFile) {
		auto& c = this->_impl->getContext(this->_impl->getDevice(platform, device), context);
		auto p = this->_impl->buildProgram(c, sourceFile);
		return 0; //TEMP
	}
}