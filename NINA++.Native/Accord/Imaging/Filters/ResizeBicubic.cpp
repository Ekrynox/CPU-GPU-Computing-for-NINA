// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2005-2011
// contacts@aforgenet.com
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

#include "ResizeBicubic.hpp"

#include "../../../ninacl_internal.hpp"

#include <algorithm>
#include <execution>
#include <ranges>



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

    static double BiCubicKernel(double x) {
        if (x < 0) x = -x;

        double biCoef = 0;
        if (x <= 1) biCoef = (1.5 * x - 2.5) * x * x + 1;
        else if (x < 2) biCoef = ((-0.5 * x + 2.5) * x - 4) * x + 2;

        return biCoef;
    }


	void ResizeBicubicGrayScale(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT) {
        double xFactor = (double)width / newWidth;
        double yFactor = (double)height / newHeight;

        int32_t ymax = height - 1;
        int32_t xmax = width - 1;

        if (__MT) {
            auto view = std::views::iota(0, newHeight);
            std::for_each(std::execution::par_unseq, view.begin(), view.end(), [src, width, height, srcStride, baseDst, newWidth, newHeight, dstStride, dstOffset, xFactor, yFactor, xmax, ymax](int32_t y) {
                // Y coordinates
                uint8_t* dst = baseDst + y * dstStride;

                // Y coordinates
                double oy = (double)y * yFactor - 0.5;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, dst++) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double g = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax)  oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            g += k2 * src[oy2 * srcStride + ox2];
                        }
                    }
                    *dst = (uint8_t)std::max(0.0, std::min(255.0, g));
                }
                });
        }
        else {
            for (int32_t y = 0; y < newHeight; y++) {
                // Y coordinates
                double oy = (double)y * yFactor - 0.5;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, baseDst++) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double g = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax)  oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            g += k2 * src[oy2 * srcStride + ox2];
                        }
                    }
                    *baseDst = (uint8_t)std::max(0.0, std::min(255.0, g));
                }
                baseDst += dstOffset;
            }
        }
	}

    void ResizeBicubicRGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT) {
        double xFactor = (double)width / newWidth;
        double yFactor = (double)height / newHeight;

        int32_t ymax = height - 1;
        int32_t xmax = width - 1;
        
        if (__MT) {
            auto view = std::views::iota(0, newHeight);
            std::for_each(std::execution::par_unseq, view.begin(), view.end(), [src, width, height, srcStride, baseDst, newWidth, newHeight, dstStride, dstOffset, xFactor, yFactor, xmax, ymax](int32_t y) {
                // Y coordinates
                uint8_t* dst = baseDst + y * dstStride;

                // Y coordinates
                double oy = (double)y * yFactor - 0.5f;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, dst += 3) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double r = 0;
                    double g = 0;
                    double b = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax) oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            // get pixel of original image
                            uint8_t* p = src + oy2 * srcStride + ox2 * 3;

                            r += k2 * p[RGB::R];
                            g += k2 * p[RGB::G];
                            b += k2 * p[RGB::B];
                        }
                    }

                    dst[RGB::R] = (uint8_t)std::max(0.0, std::min(255.0, r));
                    dst[RGB::G] = (uint8_t)std::max(0.0, std::min(255.0, g));
                    dst[RGB::B] = (uint8_t)std::max(0.0, std::min(255.0, b));
                }
                });
        }
        else {
            for (int32_t y = 0; y < newHeight; y++) {
                // Y coordinates
                double oy = (double)y * yFactor - 0.5f;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, baseDst += 3) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double r = 0;
                    double g = 0;
                    double b = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax) oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            // get pixel of original image
                            uint8_t* p = src + oy2 * srcStride + ox2 * 3;

                            r += k2 * p[RGB::R];
                            g += k2 * p[RGB::G];
                            b += k2 * p[RGB::B];
                        }
                    }

                    baseDst[RGB::R] = (uint8_t)std::max(0.0, std::min(255.0, r));
                    baseDst[RGB::G] = (uint8_t)std::max(0.0, std::min(255.0, g));
                    baseDst[RGB::B] = (uint8_t)std::max(0.0, std::min(255.0, b));
                }
                baseDst += dstOffset;
            }
        }
    }

    void ResizeBicubicARGB(uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* baseDst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset, const bool __MT) {
        double xFactor = (double)width / newWidth;
        double yFactor = (double)height / newHeight;

        int32_t ymax = height - 1;
        int32_t xmax = width - 1;

        if (__MT) {
            auto view = std::views::iota(0, newHeight);
            std::for_each(std::execution::par_unseq, view.begin(), view.end(), [src, width, height, srcStride, baseDst, newWidth, newHeight, dstStride, dstOffset, xFactor, yFactor, xmax, ymax](int32_t y) {
                // Y coordinates
                uint8_t* dst = baseDst + y * dstStride;

                double oy = (double)y * yFactor - 0.5f;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, dst += 4) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double a = 0;
                    double r = 0;
                    double g = 0;
                    double b = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax) oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            // get pixel of original image
                            uint8_t* p = src + oy2 * srcStride + ox2 * 4;

                            a += k2 * p[RGB::A];
                            r += k2 * p[RGB::R];
                            g += k2 * p[RGB::G];
                            b += k2 * p[RGB::B];
                        }
                    }

                    dst[RGB::A] = (uint8_t)std::max(0.0, std::min(255.0, a));
                    dst[RGB::R] = (uint8_t)std::max(0.0, std::min(255.0, r));
                    dst[RGB::G] = (uint8_t)std::max(0.0, std::min(255.0, g));
                    dst[RGB::B] = (uint8_t)std::max(0.0, std::min(255.0, b));
                }
                });
        }
        else {
            for (int32_t y = 0; y < newHeight; y++) {
                // Y coordinates
                double oy = (double)y * yFactor - 0.5f;
                int32_t oy1 = (int32_t)oy;
                double dy = oy - (double)oy1;

                for (int32_t x = 0; x < newWidth; x++, baseDst += 3) {
                    // X coordinates
                    double ox = (double)x * xFactor - 0.5f;
                    int32_t ox1 = (int32_t)ox;
                    double dx = ox - (double)ox1;

                    // initial pixel value
                    double a = 0;
                    double r = 0;
                    double g = 0;
                    double b = 0;

                    for (int32_t n = -1; n < 3; n++) {
                        // get Y cooefficient
                        double k1 = BiCubicKernel(dy - (double)n);

                        int32_t oy2 = oy1 + n;
                        if (oy2 < 0) oy2 = 0;
                        if (oy2 > ymax) oy2 = ymax;

                        for (int32_t m = -1; m < 3; m++) {
                            // get X cooefficient
                            double k2 = k1 * BiCubicKernel((double)m - dx);

                            int32_t ox2 = ox1 + m;
                            if (ox2 < 0) ox2 = 0;
                            if (ox2 > xmax) ox2 = xmax;

                            // get pixel of original image
                            uint8_t* p = src + oy2 * srcStride + ox2 * 3;

                            a += k2 * p[RGB::A];
                            r += k2 * p[RGB::R];
                            g += k2 * p[RGB::G];
                            b += k2 * p[RGB::B];
                        }
                    }

                    baseDst[RGB::A] = (uint8_t)std::max(0.0, std::min(255.0, a));
                    baseDst[RGB::R] = (uint8_t)std::max(0.0, std::min(255.0, r));
                    baseDst[RGB::G] = (uint8_t)std::max(0.0, std::min(255.0, g));
                    baseDst[RGB::B] = (uint8_t)std::max(0.0, std::min(255.0, b));
                }
                baseDst += dstOffset;
            }
        }
    }


    void ResizeBicubicGrayScaleOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset) {
        auto exctx = opCLM.GetImpl().getExecutionContext(context);

        auto srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, height * srcStride * sizeof(uint8_t));
        auto dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, newHeight * dstStride * sizeof(uint8_t));

        exctx.commandQ.enqueueWriteBuffer(srcBuffer, CL_TRUE, 0, height * srcStride * sizeof(uint8_t), src);

        auto vendor = exctx.device.getInfo<CL_DEVICE_VENDOR_ID>();
        cl::NDRange global;
        cl::NDRange local;

        if (vendor == 0x8086) { //Intel
            global = cl::NDRange(newHeight, newWidth);
            local = cl::NullRange;
        }
        else {
            auto maxWg = exctx.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

            int localX, localY;
            if (maxWg >= 256) localX = 16, localY = 16;
            else localX = 8, localY = 8;

            size_t globalX = ((newWidth + localX - 1) / localX) * localX;
            size_t globalY = ((newHeight + localY - 1) / localY) * localY;
            global = cl::NDRange(globalY, globalX);
            local = cl::NDRange(localY, localX);
        }

        auto kernel = cl::Kernel(exctx.programs[L"ResizeBicubic.cl"], "ResizeBicubicGrayScale");
        int arg = 0;
        kernel.setArg(arg++, srcBuffer);
        kernel.setArg(arg++, width);
        kernel.setArg(arg++, height);
        kernel.setArg(arg++, srcStride);
        kernel.setArg(arg++, dstBuffer);
        kernel.setArg(arg++, newWidth);
        kernel.setArg(arg++, newHeight);
        kernel.setArg(arg++, dstStride);
        kernel.setArg(arg++, dstOffset);

        exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
        exctx.commandQ.enqueueReadBuffer(dstBuffer, CL_TRUE, 0, newHeight * dstStride * sizeof(uint8_t), dst);
    }

    void ResizeBicubicRGBOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset) {
        auto exctx = opCLM.GetImpl().getExecutionContext(context);

        auto srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, height * srcStride * sizeof(uint8_t));
        auto dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, newHeight * dstStride * sizeof(uint8_t));

        exctx.commandQ.enqueueWriteBuffer(srcBuffer, CL_TRUE, 0, height * srcStride * sizeof(uint8_t), src);

        auto vendor = exctx.device.getInfo<CL_DEVICE_VENDOR_ID>();
        cl::NDRange global;
        cl::NDRange local;

        if (vendor == 0x8086) { //Intel
            global = cl::NDRange(newHeight, newWidth);
            local = cl::NullRange;
        }
        else {
            auto maxWg = exctx.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

            int localX, localY;
            if (maxWg >= 256) localX = 16, localY = 16;
            else localX = 8, localY = 8;

            size_t globalX = ((newWidth + localX - 1) / localX) * localX;
            size_t globalY = ((newHeight + localY - 1) / localY) * localY;
            global = cl::NDRange(globalY, globalX);
            local = cl::NDRange(localY, localX);
        }

        auto kernel = cl::Kernel(exctx.programs[L"ResizeBicubic.cl"], "ResizeBicubicRGB");
        int arg = 0;
        kernel.setArg(arg++, srcBuffer);
        kernel.setArg(arg++, width);
        kernel.setArg(arg++, height);
        kernel.setArg(arg++, srcStride);
        kernel.setArg(arg++, dstBuffer);
        kernel.setArg(arg++, newWidth);
        kernel.setArg(arg++, newHeight);
        kernel.setArg(arg++, dstStride);
        kernel.setArg(arg++, dstOffset);

        exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
        exctx.commandQ.enqueueReadBuffer(dstBuffer, CL_TRUE, 0, newHeight * dstStride * sizeof(uint8_t), dst);
    }

    void ResizeBicubicARGBOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* src, const int32_t width, const int32_t height, const int32_t srcStride, uint8_t* dst, const int32_t newWidth, const int32_t newHeight, const int32_t dstStride, const int32_t dstOffset) {
        auto exctx = opCLM.GetImpl().getExecutionContext(context);

        auto srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, height * srcStride * sizeof(uint8_t));
        auto dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, newHeight * dstStride * sizeof(uint8_t));

        exctx.commandQ.enqueueWriteBuffer(srcBuffer, CL_TRUE, 0, height * srcStride * sizeof(uint8_t), src);

        auto vendor = exctx.device.getInfo<CL_DEVICE_VENDOR_ID>();
        cl::NDRange global;
        cl::NDRange local;

        if (vendor == 0x8086) { //Intel
            global = cl::NDRange(newHeight, newWidth);
            local = cl::NullRange;
        }
        else {
            auto maxWg = exctx.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

            int localX, localY;
            if (maxWg >= 256) localX = 16, localY = 16;
            else localX = 8, localY = 8;

            size_t globalX = ((newWidth + localX - 1) / localX) * localX;
            size_t globalY = ((newHeight + localY - 1) / localY) * localY;
            global = cl::NDRange(globalY, globalX);
            local = cl::NDRange(localY, localX);
        }

        auto kernel = cl::Kernel(exctx.programs[L"ResizeBicubic.cl"], "ResizeBicubicRGBA");
        int arg = 0;
        kernel.setArg(arg++, srcBuffer);
        kernel.setArg(arg++, width);
        kernel.setArg(arg++, height);
        kernel.setArg(arg++, srcStride);
        kernel.setArg(arg++, dstBuffer);
        kernel.setArg(arg++, newWidth);
        kernel.setArg(arg++, newHeight);
        kernel.setArg(arg++, dstStride);
        kernel.setArg(arg++, dstOffset);

        exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
        exctx.commandQ.enqueueReadBuffer(dstBuffer, CL_TRUE, 0, newHeight * dstStride * sizeof(uint8_t), dst);
    }
}