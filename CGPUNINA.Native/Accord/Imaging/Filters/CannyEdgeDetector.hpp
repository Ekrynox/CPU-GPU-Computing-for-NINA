#pragma once
#include "../../../CGPUNINA.hpp"

#include <stdint.h>



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	CGPUNINA_API void CannyEdgeDetector(uint8_t* src, const int32_t srcWidth, const int32_t srcHeight, const int32_t startX, const int32_t startY, const int32_t stopX, const int32_t stopY, const int32_t width, const int32_t height, const int32_t srcStride, const int32_t srcOffset, uint8_t* dst, const int32_t dstStride, const int32_t dstOffset, const uint8_t lowThreshold, const uint8_t highThreshold);

}