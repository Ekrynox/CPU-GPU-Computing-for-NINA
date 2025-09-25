#include "ninacl.hpp"

#include "CL/opencl.hpp"



namespace LucasAlias::NINA::NinaPP {

	std::vector<cl_platform_id> getCLPlateforms() {
		cl_uint num_platforms;
		clGetPlatformIDs(0, nullptr, &num_platforms);
		auto platforms = std::vector<cl_platform_id>(num_platforms);
		clGetPlatformIDs(num_platforms, platforms.data(), nullptr);

		return platforms;
	}

	size_t getCLPlateformsNumber() {
		return getCLPlateforms().size();
	}

}