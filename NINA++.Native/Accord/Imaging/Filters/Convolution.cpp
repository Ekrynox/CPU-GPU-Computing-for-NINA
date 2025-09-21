// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
// 
// NINA++ Plugin for NINA
// https://github.com/Ekrynox/NINAPlusPlus
// 
// Copyright © Lucas Alias, 2015
// https://github.com/Ekrynox

#include "Convolution.hpp"

#include <algorithm>
#include <execution>



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

    void Process8bppImage(uint8_t* baseSrc, uint8_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int i, j, t, k, ir, jr;

        int radius = size >> 1;
        long g, div;

        int kernelSize = size * size;
        int processedKernelSize;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int i, j, t, k, ir, jr;
                long g, div;
                int processedKernelSize;

                uint8_t* src = baseSrc + y * srcStride;
                uint8_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int x = startX; x < stopX; x++, src++, dst++) {
                    g = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row or break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];

                                div += k;
                                g += k * src[ir * srcStride + jr];
                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        // all kernel elements are processed - we are not on the edge
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) g /= div;
                    g += threshold;
                    *dst = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                }
                });
        }
        else {
            // for each line
            for (int y = startY; y < stopY; y++) {
                // for each pixel
                for (int x = startX; x < stopX; x++, baseSrc++, baseDst++) {
                    g = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row or break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];

                                div += k;
                                g += k * baseSrc[ir * srcStride + jr];
                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        // all kernel elements are processed - we are not on the edge
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) g /= div;
                    g += threshold;
                    *baseDst = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                }
                baseSrc += srcOffset;
                baseDst += dstOffset;
            }
        }
    }

    void Process24bppImage(uint8_t* baseSrc, uint8_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t i, j, t, k, ir, jr;

        int32_t radius = size >> 1;
        int64_t r, g, b, div;

        int32_t kernelSize = size * size;
        int32_t processedKernelSize;

        uint8_t* p;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, pixelSize, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, div;
                int32_t processedKernelSize;
                uint8_t* p;

                uint8_t* src = baseSrc + y * srcStride;
                uint8_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += pixelSize, dst += pixelSize) {
                    r = g = b = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * pixelSize];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0)
                    {
                        r /= div;
                        g /= div;
                        b /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;

                    dst[RGB::R] = (uint8_t)((r > 255) ? 255 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint8_t)((b > 255) ? 255 : ((b < 0) ? 0 : b));

                    // take care of alpha channel
                    if (pixelSize == 4) dst[RGB::A] = src[RGB::A];
                }
                });
        }
        else {
            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                // for each pixel
                for (int32_t x = startX; x < stopX; x++, baseSrc += pixelSize, baseDst += pixelSize) {
                    r = g = b = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &baseSrc[ir * srcStride + jr * pixelSize];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0)
                    {
                        r /= div;
                        g /= div;
                        b /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;

                    baseDst[RGB::R] = (uint8_t)((r > 255) ? 255 : ((r < 0) ? 0 : r));
                    baseDst[RGB::G] = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                    baseDst[RGB::B] = (uint8_t)((b > 255) ? 255 : ((b < 0) ? 0 : b));

                    // take care of alpha channel
                    if (pixelSize == 4) baseDst[RGB::A] = baseDst[RGB::A];
                }
                baseDst += srcOffset;
                baseDst += dstOffset;
            }
        }
    }

    void Process32bppImage(uint8_t* baseSrc, uint8_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t i, j, t, k, ir, jr;

        int32_t radius = size >> 1;
        int64_t r, g, b, a, div;

        int32_t kernelSize = size * size;
        int32_t processedKernelSize;

        uint8_t* p;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, a, div;
                int32_t processedKernelSize;
                uint8_t* p;

                uint8_t* src = baseSrc + y * srcStride;
                uint8_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += 4, dst += 4) {
                    r = g = b = a = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * 4];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];
                                a += k * p[RGB::A];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0)
                    {
                        r /= div;
                        g /= div;
                        b /= div;
                        a /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;
                    a += threshold;

                    dst[RGB::R] = (uint8_t)((r > 255) ? 255 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint8_t)((b > 255) ? 255 : ((b < 0) ? 0 : b));
                    dst[RGB::A] = (uint8_t)((a > 255) ? 255 : ((a < 0) ? 0 : a));
                }
                });
        }
        else {
            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                // for each pixel
                for (int32_t x = startX; x < stopX; x++, baseSrc += 4, baseDst += 4) {
                    r = g = b = a = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &baseSrc[ir * srcStride + jr * 4];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];
                                a += k * p[RGB::A];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0)
                    {
                        r /= div;
                        g /= div;
                        b /= div;
                        a /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;
                    a += threshold;

                    baseDst[RGB::R] = (uint8_t)((r > 255) ? 255 : ((r < 0) ? 0 : r));
                    baseDst[RGB::G] = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
                    baseDst[RGB::B] = (uint8_t)((b > 255) ? 255 : ((b < 0) ? 0 : b));
                    baseDst[RGB::A] = (uint8_t)((a > 255) ? 255 : ((a < 0) ? 0 : a));
                }
                baseSrc += srcOffset;
                baseDst += dstOffset;
            }
        }
    }

    void Process16bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t i, j, t, k, ir, jr;

        int32_t radius = size >> 1;
        int64_t g, div;

        int32_t kernelSize = size * size;
        int32_t processedKernelSize;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t g, div;
                int32_t processedKernelSize;

                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src++, dst++) {
                    g = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];

                                div += k;
                                g += k * src[ir * srcStride + jr];
                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) g /= div;
                    g += threshold;
                    *dst = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                }
                });
        }
        else {
            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src++, dst++) {
                    g = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];

                                div += k;
                                g += k * src[ir * srcStride + jr];
                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) g /= div;
                    g += threshold;
                    *dst = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                }
            }
        }
    }

    void Process48bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t i, j, t, k, ir, jr;

        int32_t radius = size >> 1;
        int64_t r, g, b, div;

        int32_t kernelSize = size * size;
        int32_t processedKernelSize;

        uint16_t* p;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, pixelSize, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, div;
                int32_t processedKernelSize;
                uint16_t* p;

                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += pixelSize, dst += pixelSize) {
                    r = g = b = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * pixelSize];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) {
                        r /= div;
                        g /= div;
                        b /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;

                    dst[RGB::R] = (uint16_t)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint16_t)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));

                    // take care of alpha channel
                    if (pixelSize == 4) dst[RGB::A] = src[RGB::A];
                }
                });
        }
        else {
            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += pixelSize, dst += pixelSize) {
                    r = g = b = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * pixelSize];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) {
                        r /= div;
                        g /= div;
                        b /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;

                    dst[RGB::R] = (uint16_t)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint16_t)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));

                    // take care of alpha channel
                    if (pixelSize == 4) dst[RGB::A] = src[RGB::A];
                }
            }
        }
    }

    void Process64bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t i, j, t, k, ir, jr;

        int32_t radius = size >> 1;
        int64_t r, g, b, a, div;

        int32_t kernelSize = size * size;
        int32_t processedKernelSize;

        uint16_t* p;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, a, div;
                int32_t processedKernelSize;
                uint16_t* p;

                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += 4, dst += 4) {
                    r = g = b = a = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * 4];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];
                                a += k * p[RGB::A];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) {
                        r /= div;
                        g /= div;
                        b /= div;
                        a /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;
                    a += threshold;

                    dst[RGB::R] = (uint16_t)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint16_t)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));
                    dst[RGB::A] = (uint16_t)((a > 65535) ? 65535 : ((a < 0) ? 0 : a));
                }
                });
        }
        else {
            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                uint16_t* src = baseSrc + y * srcStride;
                uint16_t* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src += 4, dst += 4) {
                    r = g = b = a = div = processedKernelSize = 0;

                    // for each kernel row
                    for (i = 0; i < size; i++) {
                        ir = i - radius;
                        t = y + ir;

                        // skip row & break
                        if (t < startY) continue;
                        if (t >= stopY) break;

                        // for each kernel column
                        for (j = 0; j < size; j++) {
                            jr = j - radius;
                            t = x + jr;

                            // skip column
                            if (t < startX) continue;

                            if (t < stopX) {
                                k = kernel[i * size + j];
                                p = &src[ir * srcStride + jr * 4];

                                div += k;

                                r += k * p[RGB::R];
                                g += k * p[RGB::G];
                                b += k * p[RGB::B];
                                a += k * p[RGB::A];

                                processedKernelSize++;
                            }
                        }
                    }

                    // check if all kernel elements were processed
                    if (processedKernelSize == kernelSize) {
                        div = divisor;
                    }
                    else {
                        // we are on edge. do we need to use dynamic divisor or not?
                        if (!dynamicDivisorForEdges) div = divisor;
                    }

                    // check divider
                    if (div != 0) {
                        r /= div;
                        g /= div;
                        b /= div;
                        a /= div;
                    }
                    r += threshold;
                    g += threshold;
                    b += threshold;
                    a += threshold;

                    dst[RGB::R] = (uint16_t)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
                    dst[RGB::G] = (uint16_t)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
                    dst[RGB::B] = (uint16_t)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));
                    dst[RGB::A] = (uint16_t)((a > 65535) ? 65535 : ((a < 0) ? 0 : a));
                }
            }
        }
    }

}