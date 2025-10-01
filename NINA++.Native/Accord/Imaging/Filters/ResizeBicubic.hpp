#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>

#include "../../../ninacl.hpp"
#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	NINAPP_API void ResizeBicubicGrayScale(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);
	NINAPP_API void ResizeBicubicRGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);
	NINAPP_API void ResizeBicubicARGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);

	NINAPP_API void ResizeBicubicGrayScaleOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset);
}