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
#include <limits>



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

    template <typename T> NINAPP_API void Process1CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t radius = size >> 1;
        int32_t kernelSize = size * size;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t g, div;
                int32_t processedKernelSize;

                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, src++, dst++) {
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
                    *dst = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                }
                });
        }
        else {
            int32_t i, j, t, k, ir, jr;
            int64_t g, div;
            int32_t processedKernelSize;

            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

                // for each pixel
                for (int32_t x = startX; x < stopX; x++, baseSrc++, baseDst++) {
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
                    *baseDst = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                }
            }
        }
    }

    template NINAPP_API void Process1CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
    template NINAPP_API void Process1CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);


    template <typename T> NINAPP_API void Process3CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t radius = size >> 1;
        int32_t kernelSize = size * size;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, pixelSize, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, div;
                int32_t processedKernelSize;
                T* p;

                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

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

                    dst[RGB::R] = (T)((r > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((r < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : r));
                    dst[RGB::G] = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                    dst[RGB::B] = (T)((b > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((b < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : b));

                    // take care of alpha channel
                    if (pixelSize == 4) dst[RGB::A] = src[RGB::A];
                }
                });
        }
        else {
            int32_t i, j, t, k, ir, jr;
            int64_t r, g, b, div;
            int32_t processedKernelSize;
            T* p;

            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

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

                    dst[RGB::R] = (T)((r > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((r < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : r));
                    dst[RGB::G] = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                    dst[RGB::B] = (T)((b > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((b < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : b));

                    // take care of alpha channel
                    if (pixelSize == 4) dst[RGB::A] = src[RGB::A];
                }
            }
        }
    }

    template NINAPP_API void Process3CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
    template NINAPP_API void Process3CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);


    template <typename T> NINAPP_API void Process4CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT) {
        int32_t radius = size >> 1;
        int32_t kernelSize = size * size;

        if (__MT) {
            std::vector<int32_t> indices(stopY - startY);
            std::iota(indices.begin(), indices.end(), startY);
            std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel, divisor, threshold, size, dynamicDivisorForEdges, radius, kernelSize](int32_t y) {
                int32_t i, j, t, k, ir, jr;
                int64_t r, g, b, a, div;
                int32_t processedKernelSize;
                T* p;

                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

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

                    dst[RGB::R] = (T)((r > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((r < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : r));
                    dst[RGB::G] = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                    dst[RGB::B] = (T)((b > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((b < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : b));
                    dst[RGB::A] = (T)((a > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((a < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : a));
                }
                });
        }
        else {
            int32_t i, j, t, k, ir, jr;
            int64_t r, g, b, a, div;
            int32_t processedKernelSize;
            T* p;

            // for each line
            for (int32_t y = startY; y < stopY; y++) {
                T* src = baseSrc + y * srcStride;
                T* dst = baseDst + y * dstStride;

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

                    dst[RGB::R] = (T)((r > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((r < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : r));
                    dst[RGB::G] = (T)((g > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((g < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : g));
                    dst[RGB::B] = (T)((b > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((b < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : b));
                    dst[RGB::A] = (T)((a > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((a < std::numeric_limits<T>::lowest()) ? std::numeric_limits<T>::lowest() : a));
                }
            }
        }
    }

    template NINAPP_API void Process4CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
    template NINAPP_API void Process4CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);

}