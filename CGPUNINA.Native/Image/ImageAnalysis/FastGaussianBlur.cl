__kernel void boxBlurH_4OpenCL(__global unsigned char* source, __global unsigned char* dest, const int width, const int height, const int r) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y >= height || x >= width) return;

    int val = source[y * width + x];
    for (int i = 1; i <= r; i++) {
        val += source[y * width + clamp(x - i, 0, width - 1)];
        val += source[y * width + clamp(x + i, 0, width - 1)];
    }
    dest[y * width + x] = val / (2 * r + 1);
}


__kernel void boxBlurT_4OpenCL(__global unsigned char* source, __global unsigned char* dest, const int width, const int height, const int r) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y >= height || x >= width) return;

    int val = source[y * width + x];
    for (int i = 1; i <= r; i++) {
        val += source[clamp(y - i, 0, height - 1) * width + x];
        val += source[clamp(y + i, 0, height - 1) * width + x];
    }
    dest[y * width + x] = val / (2 * r + 1);
}