#pragma once
#include "../../ninapp.hpp"

#include <stdint.h>

#include "../enums.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	NINAPP_API void copyImage(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols);
	NINAPP_API void debayerPattern(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, const int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, uint16_t* Larr);

}