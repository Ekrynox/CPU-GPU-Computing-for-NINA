

__kernel void Process1CImage8bpp(__global unsigned char* baseSrc, __global unsigned char* baseDst, const int srcStride, const int dstStride, const int startX, const int startY, const int stopX, const int stopY, __constant int* kernelMat, const int divisor, const int threshold, const int size, const char dynamicDivisorForEdges) {
    int y = get_global_id(0) + startY;
    int x = get_global_id(1) + startX;
    
    int radius = size >> 1;

    __global unsigned char* src = baseSrc + y * srcStride + x;
    __global unsigned char* dst = baseDst + y * dstStride + x;
        
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