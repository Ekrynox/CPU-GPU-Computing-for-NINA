#pragma once
#include "ninacl.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include "CL/opencl.hpp"



namespace LucasAlias::NINA::NinaPP {

	class OpenCLManager::Impl {
	public:
		std::vector<cl::Platform> getPlatformList();
		size_t getPlatformNumber();

		std::vector<cl::Device> getDeviceList(const cl::Platform &platform);
		size_t getDeviceNumber(const cl::Platform &platform);

		OpenCLDeviceInfo getDeviceInfo(const cl::Device &device);

		cl::Context createContext(const std::vector<cl::Device> &devices);
		cl::CommandQueue createCommandQueue(const cl::Device& device, const cl::Context &context);
		cl::Program buildProgram(const cl::Context &context, const std::vector<std::wstring> &sourceFiles);

	friend class OpenCLManager;
	private:
		struct PlatformCompare { bool operator()(const cl::Platform& a, const cl::Platform& b) const noexcept { return a() < b(); } };
		struct DeviceCompare { bool operator()(const cl::Device& a, const cl::Device& b) const noexcept { return a() < b(); } };
		struct DeviceContextCompare { bool operator()(const std::pair<cl::Device, cl::Context>& a, const std::pair<cl::Device, cl::Context>& b) const noexcept { return a.first() == b.first() ? a.second() < b.second() : a.first() < b.first(); } };

		std::vector<cl::Platform> platforms;
		std::map<cl::Platform, std::vector<cl::Device>, PlatformCompare> devices;
		cl::Platform& getPlatform(size_t platform);
		cl::Device& getDevice(const cl::Platform& platform, size_t device);
		cl::Device& getDevice(size_t platform, size_t device);

		std::map<cl::Device, std::vector<cl::Context>, DeviceCompare> contexts;
		std::map<std::pair<cl::Device, cl::Context>, std::vector<cl::CommandQueue>, DeviceContextCompare> commandQ;
		//std::vector<cl::Program> programs;
		cl::Context& getContext(const cl::Device& device, size_t context);
		cl::Context& getContext(const cl::Platform& platform, size_t device, size_t context);
		cl::Context& getContext(size_t platform, size_t device, size_t context);
	};

}