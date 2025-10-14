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

#include "FastGaussianBlur.hpp"

#include <algorithm>
#include <execution>



void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::gaussBlur_4(uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t r, int32_t _width, int32_t _height) {
    auto bxs = boxesForGauss(r, 3);
    boxBlur_4(source, sourceLength, dest, _width, _height, (bxs[0] - 1) / 2);
    boxBlur_4(dest, sourceLength, source, _width, _height, (bxs[1] - 1) / 2);
    boxBlur_4(source, sourceLength, dest, _width, _height, (bxs[2] - 1) / 2);
}

std::vector<int32_t> LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxesForGauss(int32_t sigma, int32_t n) {
    auto wIdeal = std::sqrt((12 * sigma * sigma / n) + 1);
    auto wl = (int)std::floor(wIdeal);
    if (wl % 2 == 0) wl--;
    auto wu = wl + 2;

    auto mIdeal = (double)(12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    auto m = std::round(mIdeal);

    auto sizes = std::vector<int32_t>();
    for (auto i = 0; i < n; i++) sizes.push_back(i < m ? wl : wu);
    return sizes;
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlur_4(uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    for (auto i = 0; i < sourceLength; i++) dest[i] = source[i];
    boxBlurH_4(dest, source, w, h, r);
    boxBlurT_4(source, dest, w, h, r);
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlurH_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    auto iar = (double)1 / (r + r + 1);

    std::vector<int32_t> indices(h);
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each(std::execution::par, indices.begin(), indices.end(), [&source, &dest, w, h, r, iar](int32_t i) {
        auto ti = i * w;
        auto li = ti;
        auto ri = ti + r;
        auto fv = source[ti];
        auto lv = source[ti + w - 1];
        auto val = (r + 1) * fv;
        for (auto j = 0; j < r; j++) val += source[ti + j];
        for (auto j = 0; j <= r; j++) {
            val += source[ri++] - fv;
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (auto j = r + 1; j < w - r; j++) {
            val += source[ri++] - dest[li++];
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (auto j = w - r; j < w; j++) {
            val += lv - source[li++];
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        });
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlurT_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    auto iar = (double)1 / (r + r + 1);

    std::vector<int32_t> indices(w);
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each(std::execution::par, indices.begin(), indices.end(), [&source, &dest, w, h, r, iar](int32_t i) {
        auto ti = i;
        auto li = ti;
        auto ri = ti + r * w;
        auto fv = source[ti];
        auto lv = source[ti + w * (h - 1)];
        auto val = (r + 1) * fv;
        for (auto j = 0; j < r; j++) val += source[ti + j * w];
        for (auto j = 0; j <= r; j++) {
            val += source[ri] - fv;
            dest[ti] = (uint8_t)std::floor(val * iar);
            ri += w;
            ti += w;
        }
        for (auto j = r + 1; j < h - r; j++) {
            val += source[ri] - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ri += w;
            ti += w;
        }
        for (auto j = h - r; j < h; j++) {
            val += lv - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ti += w;
        }
    });
}