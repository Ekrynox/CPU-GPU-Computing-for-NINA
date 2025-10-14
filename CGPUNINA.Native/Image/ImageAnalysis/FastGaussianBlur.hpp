#pragma once
#include "../../CGPUNINA.hpp"

#include <cmath>
#include <stdint.h>
#include <vector>



namespace LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis {

    CGPUNINA_API void gaussBlur_4(uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t r, int32_t _width, int32_t _height);
    CGPUNINA_API std::vector<int32_t> boxesForGauss(int32_t sigma, int32_t n);
    CGPUNINA_API void boxBlur_4(uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t w, int32_t h, int32_t r);
    CGPUNINA_API void boxBlurH_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r);
    CGPUNINA_API void boxBlurT_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r);

}