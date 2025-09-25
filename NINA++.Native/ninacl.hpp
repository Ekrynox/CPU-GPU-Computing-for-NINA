#pragma once
#include "ninapp.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120


#include <vector>



namespace LucasAlias::NINA::NinaPP {

	NINAPP_API size_t getCLPlateformsNumber();

}