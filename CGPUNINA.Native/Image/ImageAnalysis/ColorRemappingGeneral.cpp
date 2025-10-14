/*
    Copyright © 2016 - 2024 Stefan Berg <isbeorn86+NINA@googlemail.com> and the N.I.N.A. contributors
    Copyright © 2025 - Lucas Alias <https://github.com/Ekrynox> (adapted to C++)

    This file is part of N.I.N.A. - Nighttime Imaging 'N' Astronomy.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Notes:
    - This file is a modified/adapted version of the original N.I.N.A. C# code.
    - Modifications include translation to C++.
*/

#include "ColorRemappingGeneral.hpp"



void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::colorRemapGrayscale(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* grayMap) {
    for (int y = 0; y < stopY; y++) {
        for (int x = 0; x < stopX; x++, ptr++) {
            *ptr = grayMap[*ptr];
        }
    }
}


void  LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::colorRemapRGB(const int32_t stopX, const int32_t stopY, uint16_t* ptr, const uint16_t* redMap, const uint16_t* greenMap, const uint16_t* blueMap) {
    for (int y = 0; y < stopY; y++) {
        for (int x = 0; x < stopX; x++, ptr += 3) {
            ptr[::RGB::R] = redMap[ptr[::RGB::R]];
            ptr[::RGB::G] = greenMap[ptr[::RGB::G]];
            ptr[::RGB::B] = blueMap[ptr[::RGB::B]];
        }
    }
}