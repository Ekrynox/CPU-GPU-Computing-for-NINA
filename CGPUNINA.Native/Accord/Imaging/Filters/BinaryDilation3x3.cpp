// Accord Imaging Library
// The Accord.NET Framework
// http://accord-framework.net
//
// Copyright © César Souza, 2009-2017
// cesarsouza at gmail.com
//
// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2005-2010
// contacts@aforgenet.com
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

#include "BinaryDilation3x3.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	void BinaryDilation3x3(uint8_t* src, const int32_t startX, const int32_t startY, const int32_t stopX, const int32_t stopY, const int32_t srcStride, const int32_t srcOffset, uint8_t* dst, const int32_t dstStride, const int32_t dstOffset) {
        // allign pointers by X and Y
        src += (startX - 1) + (startY - 1) * srcStride;
        dst += (startX - 1) + (startY - 1) * dstStride;

        // --- process the first line
        *dst = (uint8_t)(*src | src[1] | src[srcStride] | src[srcStride + 1]);

        src++;
        dst++;

        // for each pixel
        for (int32_t x = startX; x < stopX; x++, src++, dst++) {
            *dst = (uint8_t)(*src | src[-1] | src[1] | src[srcStride] | src[srcStride - 1] | src[srcStride + 1]);
        }

        *dst = (uint8_t)(*src | src[-1] | src[srcStride] | src[srcStride - 1]);

        src += srcOffset;
        dst += dstOffset;

        // --- process all lines except the last one
        for (int32_t y = startY; y < stopY; y++) {
            *dst = (uint8_t)(*src | src[1] | src[-srcStride] | src[-srcStride + 1] | src[srcStride] | src[srcStride + 1]);

            src++;
            dst++;

            // for each pixel
            for (int32_t x = startX; x < stopX; x++, src++, dst++) {
                *dst = (uint8_t)(*src | src[-1] | src[1] | src[-srcStride] | src[-srcStride - 1] | src[-srcStride + 1] |  src[srcStride] | src[srcStride - 1] | src[srcStride + 1]);
            }

            *dst = (uint8_t)(*src | src[-1] | src[-srcStride] | src[-srcStride - 1] | src[srcStride] | src[srcStride - 1]);

            src += srcOffset;
            dst += dstOffset;
        }

        // --- process the last line
        *dst = (uint8_t)(*src | src[1] | src[-srcStride] | src[-srcStride + 1]);

        src++;
        dst++;

        // for each pixel
        for (int32_t x = startX; x < stopX; x++, src++, dst++) {
            *dst = (uint8_t)(*src | src[-1] | src[1] |
                src[-srcStride] | src[-srcStride - 1] | src[-srcStride + 1]);
        }

        *dst = (uint8_t)(*src | src[-1] | src[-srcStride] | src[-srcStride - 1]);
	}

}