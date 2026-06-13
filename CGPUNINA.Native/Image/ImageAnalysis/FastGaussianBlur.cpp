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

#include "../../ninacl_internal.hpp"



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

    auto sizes = std::vector<int32_t>(n);
    for (auto i = 0; i < n; i++) sizes[i] = (i < m ? wl : wu);
    return sizes;
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlur_4(uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    std::memcpy(dest, source, sourceLength);

    double iar = (double)1 / (r + r + 1);

    std::vector<int32_t> indices(std::max(h, w));
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each_n(std::execution::par, indices.begin(), h, [&source, &dest, w, h, r, iar](int32_t i) {
        int32_t ti = i * w;
        int32_t li = ti;
        int32_t ri = ti + r;
        uint8_t fv = dest[ti];
        uint8_t lv = dest[ti + w - 1];
        int32_t val = (r + 1) * fv;
        for (int32_t j = 0; j < r; j++) val += dest[ti + j];
        for (int32_t j = 0; j <= r; j++) {
            val += dest[ri++] - fv;
            source[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (int32_t j = r + 1; j < w - r; j++) {
            val += dest[ri++] - source[li++];
            source[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (int32_t j = w - r; j < w; j++) {
            val += lv - dest[li++];
            source[ti++] = (uint8_t)std::floor(val * iar);
        }
       });

    std::for_each_n(std::execution::par, indices.begin(), w, [&source, &dest, w, h, r, iar](int32_t i) {
        int32_t ti = i;
        int32_t li = ti;
        int32_t ri = ti + r * w;
        uint8_t fv = source[ti];
        uint8_t lv = source[ti + w * (h - 1)];
        int32_t val = (r + 1) * fv;
        for (int32_t j = 0; j < r; j++) val += source[ti + j * w];
        for (int32_t j = 0; j <= r; j++) {
            val += source[ri] - fv;
            dest[ti] = (uint8_t)std::floor(val * iar);
            ri += w;
            ti += w;
        }
        for (int32_t j = r + 1; j < h - r; j++) {
            val += source[ri] - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ri += w;
            ti += w;
        }
        for (int32_t j = h - r; j < h; j++) {
            val += lv - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ti += w;
        }
        });
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlurH_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    double iar = (double)1 / (r + r + 1);

    std::vector<int32_t> indices(h);
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each(std::execution::par, indices.begin(), indices.end(), [&source, &dest, w, h, r, iar](int32_t i) {
        int32_t ti = i * w;
        int32_t li = ti;
        int32_t ri = ti + r;
        uint8_t fv = source[ti];
        uint8_t lv = source[ti + w - 1];
        int32_t val = (r + 1) * fv;
        for (int32_t j = 0; j < r; j++) val += source[ti + j];
        for (int32_t j = 0; j <= r; j++) {
            val += source[ri++] - fv;
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (int32_t j = r + 1; j < w - r; j++) {
            val += source[ri++] - dest[li++];
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        for (int32_t j = w - r; j < w; j++) {
            val += lv - source[li++];
            dest[ti++] = (uint8_t)std::floor(val * iar);
        }
        });
}

void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::boxBlurT_4(uint8_t* source, uint8_t* dest, int32_t w, int32_t h, int32_t r) {
    double iar = (double)1 / (r + r + 1);

    std::vector<int32_t> indices(w);
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each(std::execution::par, indices.begin(), indices.end(), [&source, &dest, w, h, r, iar](int32_t i) {
        int32_t ti = i;
        int32_t li = ti;
        int32_t ri = ti + r * w;
        uint8_t fv = source[ti];
        uint8_t lv = source[ti + w * (h - 1)];
        int32_t val = (r + 1) * fv;
        for (int32_t j = 0; j < r; j++) val += source[ti + j * w];
        for (int32_t j = 0; j <= r; j++) {
            val += source[ri] - fv;
            dest[ti] = (uint8_t)std::floor(val * iar);
            ri += w;
            ti += w;
        }
        for (int32_t j = r + 1; j < h - r; j++) {
            val += source[ri] - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ri += w;
            ti += w;
        }
        for (int32_t j = h - r; j < h; j++) {
            val += lv - source[li];
            dest[ti] = (uint8_t)std::floor(val * iar);
            li += w;
            ti += w;
        }
    });
}



void LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::gaussBlur_4OpenCL(OpenCLManager& opCLM, size_t context, uint8_t* source, size_t sourceLength, uint8_t* dest, int32_t r, int32_t _width, int32_t _height) {
    auto bxs = boxesForGauss(r, 3);
    


    auto exctx = opCLM.GetImpl().getExecutionContext(context);

    cl_bool unifiedMemory = false;
    exctx.device.getInfo(CL_DEVICE_HOST_UNIFIED_MEMORY, &unifiedMemory);

    cl::Buffer srcBuffer, dstBuffer, bayerBuffer;
    if (unifiedMemory) {
        srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sourceLength * sizeof(uint8_t), source, nullptr);
        dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sourceLength * sizeof(uint8_t), dest, nullptr);
    }
    else {
        srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, sourceLength * sizeof(uint8_t));
        dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, sourceLength * sizeof(uint8_t));

        exctx.commandQ.enqueueWriteBuffer(srcBuffer, CL_FALSE, 0, sourceLength * sizeof(uint8_t), source);
    }

    auto vendor = exctx.device.getInfo<CL_DEVICE_VENDOR_ID>();
    cl::NDRange global;
    cl::NDRange local;

    if (vendor == 0x8086) { //Intel
        global = cl::NDRange(_height, _width);
        local = cl::NullRange;
    }
    else {
        auto maxWg = exctx.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

        int localX, localY;
        if (maxWg >= 256) localX = 16, localY = 16;
        else localX = 8, localY = 8;

        size_t globalX = ((_width + localX - 1) / localX) * localX;
        size_t globalY = ((_height + localY - 1) / localY) * localY;
        global = cl::NDRange(globalY, globalX);
        local = cl::NDRange(localY, localX);
    }

    auto kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurH_4OpenCL");
    int arg = 0;
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[0] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

    kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurT_4OpenCL");
    arg = 0;
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[0] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);


    kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurH_4OpenCL");
    arg = 0;
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[1] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

    kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurT_4OpenCL");
    arg = 0;
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[1] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);


    kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurH_4OpenCL");
    arg = 0;
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[2] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

    kernel = cl::Kernel(exctx.programs[L"FastGaussianBlur.cl"], "boxBlurT_4OpenCL");
    arg = 0;
    kernel.setArg(arg++, dstBuffer);
    kernel.setArg(arg++, srcBuffer);
    kernel.setArg(arg++, _width);
    kernel.setArg(arg++, _height);
    kernel.setArg(arg++, (int32_t)((bxs[2] - 1) / 2));
    exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);


    cl::Event dstEvent;
    exctx.commandQ.enqueueReadBuffer(srcBuffer, CL_FALSE, 0, sourceLength * sizeof(uint8_t), dest, nullptr, &dstEvent);
    dstEvent.wait();
}