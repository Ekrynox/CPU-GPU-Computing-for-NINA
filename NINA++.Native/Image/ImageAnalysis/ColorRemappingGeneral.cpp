#include "ColorRemappingGeneral.hpp"



void LucasAlias::NINA::NinaPP::Image::ImageAnalysis::colorRemapGrayscale(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* grayMap) {
    for (int y = 0; y < stopY; y++) {
        for (int x = 0; x < stopX; x++, ptr++) {
            *ptr = grayMap[*ptr];
        }
    }
}


void  LucasAlias::NINA::NinaPP::Image::ImageAnalysis::colorRemapRGB(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* redMap, const uint16_t* greenMap, const uint16_t* blueMap) {
    for (int y = 0; y < stopY; y++) {
        for (int x = 0; x < stopX; x++, ptr += 3) {
            ptr[::RGB::R] = redMap[ptr[::RGB::R]];
            ptr[::RGB::G] = greenMap[ptr[::RGB::G]];
            ptr[::RGB::B] = blueMap[ptr[::RGB::B]];
        }
    }
}