#pragma once
#include "ninacl.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include "CL/opencl.hpp"



namespace LucasAlias::NINA::CGPUNINA {

	class OpenCLManager::Impl {
	public:
		std::vector<cl::Platform> getPlatformList();
		size_t getPlatformNumber();

		std::vector<cl::Device> getDeviceList(const cl::Platform &platform);
		size_t getDeviceNumber(const cl::Platform &platform);

		OpenCLDeviceInfo getDeviceInfo(const cl::Device &device);

		cl::Context createContext(const std::vector<cl::Device> &devices);
		cl::CommandQueue createCommandQueue(const cl::Device& device, const cl::Context &context);
		cl::Program buildProgram(const cl::Context& context, const std::wstring& sourceFile);


		struct executionContext_ {
			cl::Platform platform;
			cl::Device device;

			cl::Context context;
			cl::CommandQueue commandQ;
			std::map<std::wstring, cl::Program> programs;
		};
		typedef struct executionContext_ executionContext;
		executionContext getExecutionContext(size_t executionContext);

	friend class OpenCLManager;
	private:
		struct PlatformCompare { bool operator()(const cl::Platform& a, const cl::Platform& b) const noexcept { return a() < b(); } };

		std::vector<cl::Platform> platforms;
		std::map<cl::Platform, std::vector<cl::Device>, PlatformCompare> devices;
		cl::Platform& getPlatform(size_t platform);
		cl::Device& getDevice(const cl::Platform& platform, size_t device);
		cl::Device& getDevice(size_t platform, size_t device);

		std::map<size_t, executionContext> executionContexts;
	};

}