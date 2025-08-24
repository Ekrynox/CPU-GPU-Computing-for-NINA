// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
//
// Accord Imaging Library
// The Accord.NET Framework
// http://accord-framework.net
//
// Copyright © César Souza, 2009-2017
// cesarsouza at gmail.com
// 
// NINA++ Plugin for NINA
// https://github.com/Ekrynox/NINAPlusPlus
// 
// Copyright © Lucas Alias, 2015
// https://github.com/Ekrynox
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "BlobCounterBase.hpp"

#include "../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging {

	void collectInfo8bpp(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t stride, int32_t* objectLabels, const int32_t objectsCount,
        std::vector<int32_t>& x1, std::vector<int32_t>& y1, std::vector<int32_t>& x2, std::vector<int32_t>& y2,
        std::vector<int32_t>& area, std::vector<int64_t>& xc, std::vector<int64_t>& yc,
        std::vector<int64_t>& meanR, std::vector<int64_t>& meanG, std::vector<int64_t>& meanB,
        std::vector<int64_t>& stdDevR, std::vector<int64_t>& stdDevG, std::vector<int64_t>& stdDevB) {
        int32_t offset = stride - ImageWidth;
        int32_t i = 0;

        for (int32_t j = 1; j <= objectsCount; j++) {
            x1[j] = ImageWidth;
            y1[j] = ImageHeight;
        }

        // walk through labels array
        for (int y = 0; y < ImageHeight; y++) {
            for (int x = 0; x < ImageWidth; x++, i++, src++)
            {
                // get current label
                int32_t label = objectLabels[i];

                // skip unlabeled pixels
                if (label == 0) continue;

                // check and update all coordinates
                if (x < x1[label]) x1[label] = x;
                if (x > x2[label]) x2[label] = x;
                if (y < y1[label]) y1[label] = y;
                if (y > y2[label]) y2[label] = y;

                area[label]++;
                xc[label] += x;
                yc[label] += y;

                uint8_t g = *src;
                meanG[label] += g;
                stdDevG[label] += g * g;
            }

            src += offset;
        }

        for (int j = 1; j <= objectsCount; j++) {
            meanR[j] = meanB[j] = meanG[j];
            stdDevR[j] = stdDevB[j] = stdDevG[j];
        }
	}

    void collectInfoRGB(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t stride, const int32_t pixelSize, int32_t* objectLabels, const int32_t objectsCount,
        std::vector<int32_t>& x1, std::vector<int32_t>& y1, std::vector<int32_t>& x2, std::vector<int32_t>& y2,
        std::vector<int32_t>& area, std::vector<int64_t>& xc, std::vector<int64_t>& yc,
        std::vector<int64_t>& meanR, std::vector<int64_t>& meanG, std::vector<int64_t>& meanB,
        std::vector<int64_t>& stdDevR, std::vector<int64_t>& stdDevG, std::vector<int64_t>& stdDevB) {
        int32_t offset = stride - ImageWidth * pixelSize;
        int32_t i = 0;

        for (int32_t j = 1; j <= objectsCount; j++) {
            x1[j] = ImageWidth;
            y1[j] = ImageHeight;
        }

        // walk through labels array
        for (int32_t y = 0; y < ImageHeight; y++) {
            for (int32_t x = 0; x < ImageWidth; x++, i++, src += pixelSize) {
                // get current label
                int32_t label = objectLabels[i];

                // skip unlabeled pixels
                if (label == 0)
                    continue;

                // check and update all coordinates
                if (x < x1[label]) x1[label] = x;
                if (x > x2[label]) x2[label] = x;
                if (y < y1[label]) y1[label] = y;
                if (y > y2[label]) y2[label] = y;

                area[label]++;
                xc[label] += x;
                yc[label] += y;

                uint8_t r = src[::RGB::R];
                uint8_t g = src[::RGB::G];
                uint8_t b = src[::RGB::B];

                meanR[label] += r;
                meanG[label] += g;
                meanB[label] += b;

                stdDevR[label] += r * r;
                stdDevG[label] += g * g;
                stdDevB[label] += b * b;
            }

            src += offset;
        }
    }

}