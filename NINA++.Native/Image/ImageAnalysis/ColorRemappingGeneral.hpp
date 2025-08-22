#pragma once
#include "../../ninapp.hpp"

#include <stdint.h>

#include "../enums.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	NINAPP_API void colorRemapGrayscale(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* grayMap);
	NINAPP_API void colorRemapRGB(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* redMap, const uint16_t* greenMap, const uint16_t* blueMap);

}