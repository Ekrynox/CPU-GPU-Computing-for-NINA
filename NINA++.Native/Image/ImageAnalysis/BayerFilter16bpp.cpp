#include "BayerFilter16bpp.hpp"

#include <cmath>




void LucasAlias::NINA::NinaPP::Image::ImageAnalysis::debayerPattern(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, const int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, uint16_t* Larr) {
    int32_t widthM1 = width - 1;
    int32_t heightM1 = height - 1;

    uint32_t rgbValues[3];
    uint32_t rgbCounters[3];


    int32_t counter = 0;
    // for each line
    for (int32_t y = 0; y < height; y++) {
        // for each pixel
        for (int32_t x = 0; x < width; x++, src++, dst += 3) {
            rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
            rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

            int32_t bayerIndex = BayerPattern[(y & 1) * BPCols + (x & 1)];

            rgbValues[bayerIndex] += *src;
            rgbCounters[bayerIndex]++;

            if (x != 0) {
                bayerIndex = BayerPattern[(y & 1) * BPCols + ((x - 1) & 1)];

                rgbValues[bayerIndex] += src[-1];
                rgbCounters[bayerIndex]++;
            }

            if (x != widthM1) {
                bayerIndex = BayerPattern[(y & 1) * BPCols + ((x + 1) & 1)];

                rgbValues[bayerIndex] += src[1];
                rgbCounters[bayerIndex]++;
            }

            if (y != 0) {
                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + (x & 1)];

                rgbValues[bayerIndex] += src[-srcStride];
                rgbCounters[bayerIndex]++;

                if (x != 0) {
                    bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((x - 1) & 1)];

                    rgbValues[bayerIndex] += src[-srcStride - 1];
                    rgbCounters[bayerIndex]++;
                }

                if (x != widthM1) {
                    bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((x + 1) & 1)];

                    rgbValues[bayerIndex] += src[-srcStride + 1];
                    rgbCounters[bayerIndex]++;
                }
            }

            if (y != heightM1) {
                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + (x & 1)];

                rgbValues[bayerIndex] += src[srcStride];
                rgbCounters[bayerIndex]++;

                if (x != 0) {
                    bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + ((x - 1) & 1)];

                    rgbValues[bayerIndex] += src[srcStride - 1];
                    rgbCounters[bayerIndex]++;
                }

                if (x != widthM1) {
                    bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + ((x + 1) & 1)];
                    rgbValues[bayerIndex] += src[srcStride + 1];
                    rgbCounters[bayerIndex]++;
                }
            }

            dst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
            dst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
            dst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
            if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                Rarr[counter] = dst[RGB::B];
                Garr[counter] = dst[RGB::G];
                Barr[counter] = dst[RGB::R];
            }
            if (Larr != nullptr) {
                Larr[counter] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
            }
            counter++;
        }
        src += srcOffset;
        dst += dstOffset;
    }
    
}