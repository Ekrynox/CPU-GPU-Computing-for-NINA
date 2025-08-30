#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>

#include "../../../Image/enums.hpp"
#include "../../../Image/struct.hpp"


namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	NINAPP_API int32_t CalculateSISThreshold(uint8_t* ptr, const int32_t startX, const int32_t startY, const int32_t width, const int32_t height, const int32_t stride);

}