#include "ninacl_internal.hpp"

#include <filesystem>
#include <fstream>



namespace LucasAlias::NINA::NinaPP {

	OpenCLManager::OpenCLManager() : _impl(std::make_unique<Impl>()) {
		refreshPlatformList();
	}
	OpenCLManager::~OpenCLManager() {
		this->_impl->executionContexts.clear();

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


	size_t OpenCLManager::createExecutionContext(size_t platform, size_t device, const std::wstring& sourcePath, const std::vector<std::wstring>& sourceFiles) {
		Impl::executionContext exctx;
		exctx.platform = this->_impl->getPlatform(platform);
		exctx.device = this->_impl->getDevice(exctx.platform, device);

		exctx.context = this->_impl->createContext(std::vector({ exctx.device }));
		exctx.commandQ = this->_impl->createCommandQueue(exctx.device, exctx.context);

		exctx.programs = std::map<std::wstring, cl::Program>();
		auto sp = std::filesystem::path(sourcePath);
		for (const auto& sf : sourceFiles) {
			exctx.programs[sf] = this->_impl->buildProgram(exctx.context, sp / std::filesystem::path(sf));
		}

		size_t nextId = this->_impl->executionContexts.empty() ? 0 : std::prev(this->_impl->executionContexts.end())->first + 1;
		this->_impl->executionContexts[nextId] = exctx;
		return nextId;
	}
	OpenCLManager::Impl::executionContext OpenCLManager::Impl::getExecutionContext(size_t executionContext) {
		if (!this->executionContexts.contains(executionContext)) throw OpenCLContextNotFound();
		return this->executionContexts[executionContext];
	}
}