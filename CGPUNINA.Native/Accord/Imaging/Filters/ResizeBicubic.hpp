#pragma once
#include "../../../CGPUNINA.hpp"

#include <stdint.h>

#include "../../../ninacl.hpp"
#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	CGPUNINA_API void ResizeBicubicGrayScale(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);
	CGPUNINA_API void ResizeBicubicRGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);
	CGPUNINA_API void ResizeBicubicARGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT);

	CGPUNINA_API void ResizeBicubicGrayScaleOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset);
	CGPUNINA_API void ResizeBicubicRGBOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset);
	CGPUNINA_API void ResizeBicubicARGBOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset);
}