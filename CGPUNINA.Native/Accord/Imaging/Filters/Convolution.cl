#define RGB_R 2
#define RGB_G 1
#define RGB_B 0
#define RGB_A 3


__kernel void Process1CImage8bpp_GLOBAL(__global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;
    
    int radius = size >> 1;

    __global unsigned char* src = baseSrc + (y - startY) * srcStride + x;
    __global unsigned char* dst = baseDst + (y - startY) * dstStride + x;
        
    int i, j, k, ir, jr;
    long g = 0;
    long div = 0;
    int processedKernelSize = 0;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;
            g += k * src[ir * srcStride + jr];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        g /= divisor;
    }
    else if (div != 0) g /= div;

    g += threshold;
    *dst = (unsigned char)((g > 255) ? 255 : ((g < 0) ? 0 : g));
}

//Not use as there are similar performance with the global version
__kernel void Process1CImage8bpp_LOCAL(const int LOCAL_X, const int LOCAL_Y, __local unsigned char* tile, __global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int ly = get_local_id(0);
    int lx = get_local_id(1);

    int radius = size >> 1;

    __global unsigned char* src = baseSrc + (y - startY) * srcStride + x;
    __global unsigned char* dst = baseDst + (y - startY) * dstStride + x;

    int i, j, k, ir, jr;
    long g = 0;
    long div = 0;
    int processedKernelSize = 0;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    int lstride = LOCAL_X + size;

    // Fill the local tile
    tile[(ly + radius) * lstride + (lx + radius)] = src[0];
    if (lx == 0 && x > startX) {
        for (j = radius - jmin; j > 0; j--) {
            tile[(ly + radius) * lstride + (lx + radius - j)] = src[-j];
        }
    }
    if (lx == LOCAL_X && x < stopX - 1) {
        for (j = jmax - radius; j > 0; j--) {
            tile[(ly + radius) * lstride + (lx + radius + j)] = src[j];
        }
    }
    
    if (ly == 0 && y > startY) {
        for (i = radius - imin; i > 0; i--) {
            tile[(ly + radius - i) * lstride + (lx + radius)] = src[-i * srcStride];
        }

        if (lx == 0 && x > startX) {
            for (i = radius - imin; i > 0; i--) {
                for (j = radius - jmin; j > 0; j--) {
                    tile[(ly + radius - i) * lstride + (lx + radius - j)] = src[-i * srcStride - j];
                }
            }
        }
        if (lx == LOCAL_X && x < stopX - 1) {
            for (i = radius - imin; i > 0; i--) {
                for (j = jmax - radius; j > 0; j--) {
                    tile[(ly + radius - i) * lstride + (lx + radius + j)] = src[-i * srcStride + j];
                }
            }
        }
    }
    if (ly == LOCAL_Y && y < stopY - 1) {
        for (i = imax - radius; i > 0; i--) {
            tile[(ly + radius + i) * lstride + (lx + radius)] = src[i * srcStride];
        }

        if (lx == 0 && x > startX) {
            for (i = imax - radius; i > 0; i--) {
                for (j = radius - jmin; j > 0; j--) {
                    tile[(ly + radius + i) * lstride + (lx + radius - j)] = src[i * srcStride - j];
                }
            }
        }
        if (lx == LOCAL_X && x < stopX - 1) {
            for (i = imax - radius; i > 0; i--) {
                for (j = jmax - radius; j > 0; j--) {
                    tile[(ly + radius + i) * lstride + (lx + radius + j)] = src[i * srcStride + j];
                }
            }
        }
    }
    
    barrier(CLK_LOCAL_MEM_FENCE);



    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;
            g += k * tile[(ly + radius + ir) * lstride + (lx + radius + jr)];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        g /= divisor;
    }
    else if (div != 0) g /= div;

    g += threshold;
    *dst = (unsigned char)((g > 255) ? 255 : ((g < 0) ? 0 : g));
}


__kernel void Process1CImage16bpp_GLOBAL(__global unsigned short* baseSrc, __global unsigned short* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int radius = size >> 1;

    __global unsigned short* src = baseSrc + (y - startY) * srcStride + x;
    __global unsigned short* dst = baseDst + (y - startY) * dstStride + x;

    int i, j, k, ir, jr;
    long g = 0;
    long div = 0;
    int processedKernelSize = 0;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;
            g += k * src[ir * srcStride + jr];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        g /= divisor;
    }
    else if (div != 0) g /= div;

    g += threshold;
    *dst = (unsigned short)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
}



__kernel void Process3CImage8bpp_GLOBAL(__global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, const int pixelSize, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int radius = size >> 1;

    __global unsigned char* src = baseSrc + (y - startY) * srcStride + x * pixelSize;
    __global unsigned char* dst = baseDst + (y - startY) * dstStride + x * pixelSize;

    int i, j, k, ir, jr;
    long r = 0;
    long g = 0;
    long b = 0;
    long div = 0;
    int processedKernelSize = 0;
    __global unsigned char* p;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;

            p = &src[ir * srcStride + jr * pixelSize];
            r += k * p[RGB_R];
            g += k * p[RGB_G];
            b += k * p[RGB_B];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        r /= divisor;
        g /= divisor;
        b /= divisor;
    }
    else if (div != 0) {
        r /= div;
        g /= div;
        b /= div;
    }

    r += threshold;
    g += threshold;
    b += threshold;

    dst[RGB_R] = (unsigned char)((r > 255) ? 255 : ((r < 0) ? 0 : r));
    dst[RGB_G] = (unsigned char)((g > 255) ? 255 : ((g < 0) ? 0 : g));
    dst[RGB_B] = (unsigned char)((b > 255) ? 255 : ((b < 0) ? 0 : b));
    if (pixelSize == 4) dst[RGB_A] = src[RGB_A];
}

__kernel void Process3CImage16bpp_GLOBAL(__global unsigned short* baseSrc, __global unsigned short* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, const int pixelSize, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int radius = size >> 1;

    __global unsigned short* src = baseSrc + (y - startY) * srcStride + x * pixelSize;
    __global unsigned short* dst = baseDst + (y - startY) * dstStride + x * pixelSize;

    int i, j, k, ir, jr;
    long r = 0;
    long g = 0;
    long b = 0;
    long div = 0;
    int processedKernelSize = 0;
    __global unsigned short* p;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;

            p = &src[ir * srcStride + jr * pixelSize];
            r += k * p[RGB_R];
            g += k * p[RGB_G];
            b += k * p[RGB_B];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        r /= divisor;
        g /= divisor;
        b /= divisor;
    }
    else if (div != 0) {
        r /= div;
        g /= div;
        b /= div;
    }

    r += threshold;
    g += threshold;
    b += threshold;

    dst[RGB_R] = (unsigned char)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
    dst[RGB_G] = (unsigned char)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
    dst[RGB_B] = (unsigned char)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));
    if (pixelSize == 4) dst[RGB_A] = src[RGB_A];
}



__kernel void Process4CImage8bpp_GLOBAL(__global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int radius = size >> 1;

    __global unsigned char* src = baseSrc + (y - startY) * srcStride + x * 4;
    __global unsigned char* dst = baseDst + (y - startY) * dstStride + x * 4;

    int i, j, k, ir, jr;
    long r = 0;
    long g = 0;
    long b = 0;
    long a = 0;
    long div = 0;
    int processedKernelSize = 0;
    __global unsigned char* p;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;

            p = &src[ir * srcStride + jr * 4];
            r += k * p[RGB_R];
            g += k * p[RGB_G];
            b += k * p[RGB_B];
            a += k * p[RGB_A];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        r /= divisor;
        g /= divisor;
        b /= divisor;
        a /= divisor;
    }
    else if (div != 0) {
        r /= div;
        g /= div;
        b /= div;
        a /= div;
    }

    r += threshold;
    g += threshold;
    b += threshold;
    a += threshold;

    dst[RGB_R] = (unsigned char)((r > 255) ? 255 : ((r < 0) ? 0 : r));
    dst[RGB_G] = (unsigned char)((g > 255) ? 255 : ((g < 0) ? 0 : g));
    dst[RGB_B] = (unsigned char)((b > 255) ? 255 : ((b < 0) ? 0 : b));
    dst[RGB_A] = (unsigned char)((a > 255) ? 255 : ((a < 0) ? 0 : a));
}

__kernel void Process4CImage16bpp_GLOBAL(__global unsigned short* baseSrc, __global unsigned short* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;

    if (y >= stopY || x >= stopX) return;

    int radius = size >> 1;

    __global unsigned short* src = baseSrc + (y - startY) * srcStride + x * 4;
    __global unsigned short* dst = baseDst + (y - startY) * dstStride + x * 4;

    int i, j, k, ir, jr;
    long r = 0;
    long g = 0;
    long b = 0;
    long a = 0;
    long div = 0;
    int processedKernelSize = 0;
    __global unsigned short* p;

    int imin = max(0, startY - y + radius);
    int imax = min(size, stopY - y + radius);
    int jmin = max(0, startX - x + radius);
    int jmax = min(size, stopX - x + radius);

    // for each kernel row
    for (i = imin; i < imax; i++) {
        ir = i - radius;

        // for each kernel column
        for (j = jmin; j < jmax; j++) {
            jr = j - radius;

            k = kernelMat[i * size + j];

            div += k;

            p = &src[ir * srcStride + jr * 4];
            r += k * p[RGB_R];
            g += k * p[RGB_G];
            b += k * p[RGB_B];
            a += k * p[RGB_A];
            processedKernelSize++;
        }
    }

    if ((processedKernelSize == size * size) || !dynamicDivisorForEdges) { //Not on Edge or no dynamic divisor
        r /= divisor;
        g /= divisor;
        b /= divisor;
        a /= divisor;
    }
    else if (div != 0) {
        r /= div;
        g /= div;
        b /= div;
        a /= div;
    }

    r += threshold;
    g += threshold;
    b += threshold;
    a += threshold;

    dst[RGB_R] = (unsigned short)((r > 65535) ? 65535 : ((r < 0) ? 0 : r));
    dst[RGB_G] = (unsigned short)((g > 65535) ? 65535 : ((g < 0) ? 0 : g));
    dst[RGB_B] = (unsigned short)((b > 65535) ? 65535 : ((b < 0) ? 0 : b));
    dst[RGB_A] = (unsigned short)((a > 65535) ? 65535 : ((a < 0) ? 0 : a));
}