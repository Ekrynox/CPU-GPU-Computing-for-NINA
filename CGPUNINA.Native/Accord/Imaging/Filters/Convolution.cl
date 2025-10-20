

__kernel void Process1CImage8bpp(__global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;
    
    int radius = size >> 1;
    int kernelSize = size * size;

    __global unsigned char* src = baseSrc + y * srcStride + x;
    __global unsigned char* dst = baseDst + y * dstStride + x;
        
    int i, j, t, k, ir, jr;
    long g = 0;
    long div = 0;
    int processedKernelSize = 0;

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
                k = kernelMat[i * size + j];

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
    *dst = (unsigned char)((g > 255) ? 255 : ((g < 0) ? 0 : g));
}