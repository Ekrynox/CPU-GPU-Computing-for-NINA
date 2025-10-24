#define RGB_R 2
#define RGB_G 1
#define RGB_B 0
#define RGB_A 3


/*static inline float BiCubicKernel(float x) {
    x = fabs(x);

    if (x <= 1.0f) return (1.5f * x - 2.5f) * x * x + 1.0f;
    else if (x < 2.0f) return ((-0.5f * x + 2.5f) * x - 4.0f) * x + 2.0f;
    else return 0.0f;
}*/

static inline float4 BiCubicKernelVec4(float4 x) {
    x = fabs(x);

    return convert_float4(x <= 1.0f) * ((1.5f * x - 2.5f) * x * x + 1.0f) + convert_float4((x > 1.0f) & (x < 2.0f)) * (((-0.5f * x + 2.5f) * x - 4.0f) * x + 2.0f);
}


__constant int4 kernelOffsets = (int4)(-1, 0, 1, 2);
__constant float4 kernelOffsetsf = (float4)(-1.0f, 0.0f, 1.0f, 2.0f);


__kernel void ResizeBicubicGrayScale(__global uchar* src, const int widthM1, const int heightM1, const int srcStride, __global uchar* dst, const float xFactor, const float yFactor, const int dstStride, const int dstOffset) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y > heightM1 || x > widthM1) return;

    // Y coordinates
    float oy = (float)y * yFactor - 0.5f;
    int oy1 = (int)oy;
    float dy = oy - (float)oy1;

    // X coordinates
    float ox = (float)x * xFactor - 0.5f;
    int ox1 = (int)ox;
    float dx = ox - (float)ox1;

    // initial pixel value
    float g = 0;

    float4 k1V = BiCubicKernelVec4(dy - kernelOffsetsf);
    float4 k2V = BiCubicKernelVec4(kernelOffsetsf - dx);

    int4 oy2V = clamp(oy1 + kernelOffsets, 0, heightM1) * srcStride;
    int4 ox2V = clamp(ox1 + kernelOffsets, 0, widthM1);

    for (int n = 0; n < 4; n++) {
        float4 k1 = k1V[n] * k2V;
        int4 oy2 = oy2V[n] + ox2V;

        for (int m = 0; m < 4; m++) {
            g += k1[m] * src[oy2[m]];
        }
    }
    dst[y * dstStride + x] = (uchar)clamp(g, 0.0f, 255.0f);
}


__kernel void ResizeBicubicRGB(__global uchar* src, const int widthM1, const int heightM1, const int srcStride, __global uchar* dst, const float xFactor, const float yFactor, const int dstStride, const int dstOffset) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y > heightM1 || x > widthM1) return;

    // Y coordinates
    float oy = (float)y * yFactor - 0.5f;
    int oy1 = (int)oy;
    float dy = oy - (float)oy1;

    // X coordinates
    float ox = (float)x * xFactor - 0.5f;
    int ox1 = (int)ox;
    float dx = ox - (float)ox1;

    // initial pixel value
    float r = 0;
    float g = 0;
    float b = 0;

    float4 k1V = BiCubicKernelVec4(dy - kernelOffsetsf);
    float4 k2V = BiCubicKernelVec4(kernelOffsetsf - dx);

    int4 oy2V = clamp(oy1 + kernelOffsets, 0, heightM1) * srcStride;
    int4 ox2V = clamp(ox1 + kernelOffsets, 0, widthM1) * 3;

    for (int n = 0; n < 4; n++) {
        float4 k1 = k1V[n] * k2V;
        int4 oy2 = oy2V[n] + ox2V;

        for (int m = 0; m < 4; m++) {
            float k2 = k1[m];
            __global uchar* p = src + oy2[m];

            r += k2 * p[RGB_R];
            g += k2 * p[RGB_G];
            b += k2 * p[RGB_B];
        }
    }

    __global uchar* p = dst + y * dstStride + x;
    p[RGB_R] = (uchar)clamp(r, 0.0f, 255.0f);
    p[RGB_G] = (uchar)clamp(g, 0.0f, 255.0f);
    p[RGB_B] = (uchar)clamp(b, 0.0f, 255.0f);
}


__kernel void ResizeBicubicARGB(__global uchar* src, const int widthM1, const int heightM1, const int srcStride, __global uchar* dst, const float xFactor, const float yFactor, const int dstStride, const int dstOffset) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y > heightM1 || x > widthM1) return;

    // Y coordinates
    float oy = (float)y * yFactor - 0.5f;
    int oy1 = (int)oy;
    float dy = oy - (float)oy1;

    // X coordinates
    float ox = (float)x * xFactor - 0.5f;
    int ox1 = (int)ox;
    float dx = ox - (float)ox1;

    // initial pixel value
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;

    float4 k1V = BiCubicKernelVec4(dy - kernelOffsetsf);
    float4 k2V = BiCubicKernelVec4(kernelOffsetsf - dx);

    int4 oy2V = clamp(oy1 + kernelOffsets, 0, heightM1) * srcStride;
    int4 ox2V = clamp(ox1 + kernelOffsets, 0, widthM1) * 4;

    for (int n = 0; n < 4; n++) {
        float4 k1 = k1V[n] * k2V;
        int4 oy2 = oy2V[n] + ox2V;

        for (int m = 0; m < 4; m++) {
            float k2 = k1[m];
            __global uchar* p = src + oy2[m];

            r += k2 * p[RGB_R];
            g += k2 * p[RGB_G];
            b += k2 * p[RGB_B];
            a += k2 * p[RGB_A];
        }
    }

    __global uchar* p = dst + y * dstStride + x;
    p[RGB_R] = (uchar)clamp(r, 0.0f, 255.0f);
    p[RGB_G] = (uchar)clamp(g, 0.0f, 255.0f);
    p[RGB_B] = (uchar)clamp(b, 0.0f, 255.0f);
    p[RGB_A] = (uchar)clamp(a, 0.0f, 255.0f);
}