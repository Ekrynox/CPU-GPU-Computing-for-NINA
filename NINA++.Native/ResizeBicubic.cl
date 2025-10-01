#define RGB_R 2
#define RGB_G 1
#define RGB_B 0


float BiCubicKernel(float x) {
    x = fabs(x);
    return (x <= 1) * ((1.5 * x - 2.5) * x * x + 1) + (x > 1) * (x < 2) * (((-0.5 * x + 2.5) * x - 4) * x + 2);
}


__kernel void ResizeBicubicGrayScale(__global uchar* src, const int width, const int height, const int srcStride, __global uchar* dst, const int newWidth, const int newHeight, const int dstStride, const int dstOffset) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    float xFactor = (float)width / newWidth;
    float yFactor = (float)height / newHeight;

    int ymax = height - 1;
    int xmax = width - 1;

    if (y >= newHeight || x >= newWidth) return;

    // Y coordinates
    float oy = (float)y * yFactor - 0.5;
    int oy1 = (int)oy;
    float dy = oy - (float)oy1;

    // X coordinates
    float ox = (float)x * xFactor - 0.5f;
    int ox1 = (int)ox;
    float dx = ox - (float)ox1;

    // initial pixel value
    float g = 0;

    for (int n = -1; n < 3; n++) {
        // get Y cooefficient
        float k1 = BiCubicKernel(dy - (float)n);

        int oy2 = clamp(oy1 + n, 0, ymax);

        for (int m = -1; m < 3; m++) {
            // get X cooefficient
            float k2 = k1 * BiCubicKernel((float)m - dx);

            int ox2 = clamp(ox1 + m, 0, xmax);

            g += k2 * src[oy2 * srcStride + ox2];
        }
    }
    dst[y * dstStride + x] = clamp((uchar)g, (uchar)0, (uchar)255);
}