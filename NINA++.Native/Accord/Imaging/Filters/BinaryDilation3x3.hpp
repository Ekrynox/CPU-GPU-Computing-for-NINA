#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	NINAPP_API void BinaryDilation3x3(uint8_t* src, const int32_t startX, const int32_t startY, const int32_t stopX, const int32_t stopY, const int32_t srcStride, const int32_t srcOffset, uint8_t* dst, const int32_t dstStride, const int32_t dstOffset);

}