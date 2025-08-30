#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>

#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	NINAPP_API void ResizeBicubicGrayScale(uint8_t* src, int32_t width, int32_t height, int32_t srcStride, uint8_t* dst, int32_t newWidth, int32_t newHeight, int32_t dstOffset);
	NINAPP_API void ResizeBicubicRGB(uint8_t* src, int32_t width, int32_t height, int32_t srcStride, uint8_t* dst, int32_t newWidth, int32_t newHeight, int32_t dstOffset);
	NINAPP_API void ResizeBicubicARGB(uint8_t* src, int32_t width, int32_t height, int32_t srcStride, uint8_t* dst, int32_t newWidth, int32_t newHeight, int32_t dstOffset);

}