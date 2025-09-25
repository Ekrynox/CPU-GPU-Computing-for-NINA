#pragma once
#include "ninacl.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include "CL/opencl.hpp"



namespace LucasAlias::NINA::NinaPP {

	class OpenCLManager::Impl {
	public:
		void refreshDeviceList(cl_platform_id platform);
		size_t getDeviceNumber(cl_platform_id platform);

	friend class OpenCLManager;
	private:
		std::vector<cl_platform_id> platforms;
		std::map<cl_platform_id, std::vector<cl_device_id>> devices;
	};

}