#pragma once
#include "../../CGPUNINA.hpp"

#include <stdint.h>

#include "../enums.hpp"



namespace LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis {

	CGPUNINA_API void colorRemapGrayscale(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* grayMap);
	CGPUNINA_API void colorRemapRGB(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* redMap, const uint16_t* greenMap, const uint16_t* blueMap);

}