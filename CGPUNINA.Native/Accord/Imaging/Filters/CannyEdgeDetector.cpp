// AForge Image Processing Library
// AForge.NET framework
//
// Copyright © Andrew Kirillov, 2005-2008
// andrew.kirillov@aforgenet.com
//
// Article by Bill Green was used as the reference
// http://www.pages.drexel.edu/~weg22/can_tut.html
//
// NINA++ Plugin for NINA
// https://github.com/Ekrynox/NINAPlusPlus
// 
// Copyright © Lucas Alias, 2015
// https://github.com/Ekrynox

#include "CannyEdgeDetector.hpp"

#include <algorithm>
#include <limits>
#include <math.h>
#include <numbers>



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	void CannyEdgeDetector(uint8_t* src, const int32_t srcWidth, const int32_t srcHeight, const int32_t startX, const int32_t startY, const int32_t stopX, const int32_t stopY, const int32_t width, const int32_t height, const int32_t srcStride, const int32_t srcOffset, uint8_t* dst, const int32_t dstStride, const int32_t dstOffset, const uint8_t lowThreshold, const uint8_t highThreshold) {
        int32_t gx, gy;
        double orientation, toAngle = 180.0 / std::numbers::pi;
        float leftPixel = 0, rightPixel = 0;

        // orientation array
        uint8_t* orients = new uint8_t[width * height];
        // gradients array
        float* gradients = new float[srcWidth * srcHeight];
        float maxGradient = -std::numeric_limits<float>::infinity();

        // allign pointer
        src += srcStride * startY + startX;

        // STEP 2 - calculate magnitude and edge orientation
        int32_t p = 0;

        // for each line
        for (int32_t y = startY; y < stopY; y++) {
            // for each pixel
            for (int32_t x = startX; x < stopX; x++, src++, p++) {
                gx = src[-srcStride + 1] + src[srcStride + 1] - src[-srcStride - 1] - src[srcStride - 1] + 2 * (src[1] - src[-1]);

                gy = src[-srcStride - 1] + src[-srcStride + 1] - src[srcStride - 1] - src[srcStride + 1] + 2 * (src[-srcStride] - src[srcStride]);

                // get gradient value
                gradients[x + srcWidth * y] = (float)sqrt(gx * gx + gy * gy);
                if (gradients[x + srcWidth * y] > maxGradient) maxGradient = gradients[x + srcWidth * y];

                // --- get orientation
                if (gx == 0) orientation = (gy == 0) ? 0 : 90;
                else {
                    double div = (double)gy / gx;

                    // handle angles of the 2nd and 4th quads
                    if (div < 0) orientation = 180 - atan(-div) * toAngle;
                    // handle angles of the 1st and 3rd quads
                    else orientation = atan(div) * toAngle;

                    // get closest angle from 0, 45, 90, 135 set
                    if (orientation < 22.5) orientation = 0;
                    else if (orientation < 67.5) orientation = 45;
                    else if (orientation < 112.5) orientation = 90;
                    else if (orientation < 157.5) orientation = 135;
                    else orientation = 0;
                }

                // save orientation
                orients[p] = (uint8_t)orientation;
            }
            src += srcOffset;
        }

        // STEP 3 - suppres non maximums
        // allign pointer
        uint8_t* dsta = dst + dstStride * startY + startX;

        p = 0;

        // for each line
        for (int32_t y = startY; y < stopY; y++) {
            // for each pixel
            for (int32_t x = startX; x < stopX; x++, dsta++, p++) {
                // get two adjacent pixels
                switch (orients[p]) {
                case 0:
                    leftPixel = gradients[(x - 1) + srcWidth * y];
                    rightPixel = gradients[(x + 1) + srcWidth * y];
                    break;
                case 45:
                    leftPixel = gradients[(x - 1) + srcWidth * (y + 1)];
                    rightPixel = gradients[(x + 1) + srcWidth * (y - 1)];
                    break;
                case 90:
                    leftPixel = gradients[x + srcWidth * (y + 1)];
                    rightPixel = gradients[x + srcWidth * (y - 1)];
                    break;
                case 135:
                    leftPixel = gradients[(x + 1) + srcWidth * (y + 1)];
                    rightPixel = gradients[(x - 1) + srcWidth * (y - 1)];
                    break;
                }
                // compare current pixels value with adjacent pixels
                if ((gradients[x + srcWidth * y] < leftPixel) || (gradients[x + srcWidth * y] < rightPixel)) *dsta = 0;
                else *dsta = (uint8_t)(gradients[x + srcWidth * y] / maxGradient * 255);
            }
            dsta += dstOffset;
        }

        // STEP 4 - hysteresis
        // allign pointer
        uint8_t* dstb = dst + dstStride * startY + startX;

        // for each line
        for (int y = startY; y < stopY; y++) {
            // for each pixel
            for (int x = startX; x < stopX; x++, dstb++) {
                if (*dstb < highThreshold) {
                    if (*dstb < lowThreshold) *dstb = 0;
                    else if ((dstb[-1] < highThreshold) && (dstb[1] < highThreshold) && (dstb[-dstStride - 1] < highThreshold) && (dstb[-dstStride] < highThreshold) && (dstb[-dstStride + 1] < highThreshold) && (dstb[dstStride - 1] < highThreshold) && (dstb[dstStride] < highThreshold) && (dstb[dstStride + 1] < highThreshold)) *dstb = 0;  
                }
            }
            dstb += dstOffset;
        }

        delete[] orients;
        delete[] gradients;
	}

}