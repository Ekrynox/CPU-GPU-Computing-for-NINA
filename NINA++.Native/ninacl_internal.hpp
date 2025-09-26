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

		cl::Context createContext(std::vector<cl::Device> &devices);
		cl::CommandQueue createCommandQueue(cl::Context &context, cl::Device &device);
		cl::Program buildProgram(cl::Context &context, const std::vector<std::wstring> &sourceFiles);

	friend class OpenCLManager;
	private:
		struct PlatformCompare { bool operator()(const cl::Platform& a, const cl::Platform& b) const noexcept { return a() < b(); } };

		std::vector<cl::Platform> platforms;
		std::map<cl::Platform, std::vector<cl::Device>, PlatformCompare> devices;
	};

}