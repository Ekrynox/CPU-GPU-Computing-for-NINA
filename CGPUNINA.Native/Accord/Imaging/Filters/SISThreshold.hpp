#pragma once
#include "../../../CGPUNINA.hpp"

#include <stdint.h>

#include "../../../Image/enums.hpp"
#include "../../../Image/struct.hpp"


namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	CGPUNINA_API int32_t CalculateSISThreshold(uint8_t* ptr, const int32_t startX, const int32_t startY, const int32_t width, const int32_t height, const int32_t stride);

}