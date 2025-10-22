#define RGB_R 2
#define RGB_G 1
#define RGB_B 0

__kernel void debayerPattern(const int width, const int height, __global unsigned short* src, __global unsigned short* dst, const int srcStride, const int srcOffset, const int dstOffset, __constant int* BayerPattern) {
    int y = get_global_id(0);
    int x = get_global_id(1);

    if (y >= height || x >= width) return;

    __global unsigned short* tmpsrc = src + y * srcStride + x;
    __global unsigned short* tmpdst = dst + y * (3 * width + dstOffset) + x * 3;
    unsigned int counter = y * width + x;

    // Debayering
    unsigned int rgbValues[3];
    unsigned int rgbCounters[3];
    unsigned int bayerIndex;

    rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
    rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

    int yParity = y & 1;
    int yParityInv = yParity ^ 1;
    int xParity = x & 1;
    int xParityInv = xParity ^ 1;

    if ((y > 0) && (y < (height - 1)) && (x > 0) && (x < (width - 1))) {
        bayerIndex = BayerPattern[yParity * 2 + xParity];
        rgbValues[bayerIndex] += *tmpsrc;
        rgbCounters[bayerIndex]++;

        //LEFT & RIGHT
        bayerIndex = BayerPattern[yParity * 2 + xParityInv];
        rgbValues[bayerIndex] += tmpsrc[-1] + tmpsrc[1];
        rgbCounters[bayerIndex] += 2;

        //UP & DOWN
        bayerIndex = BayerPattern[yParityInv * 2 + xParity];
        rgbValues[bayerIndex] += tmpsrc[-srcStride] + tmpsrc[srcStride];
        rgbCounters[bayerIndex] += 2;

        //CORNERS
        bayerIndex = BayerPattern[yParityInv * 2 + xParityInv];
        rgbValues[bayerIndex] += tmpsrc[-srcStride - 1] + tmpsrc[-srcStride + 1] + tmpsrc[srcStride - 1] + tmpsrc[srcStride + 1];
        rgbCounters[bayerIndex] += 4;
    }
    else {
        bayerIndex = BayerPattern[yParity * 2 + xParity];
        rgbValues[bayerIndex] += *tmpsrc;
        rgbCounters[bayerIndex]++;

        if (x > 0) {
            bayerIndex = BayerPattern[yParity * 2 + xParityInv];
            rgbValues[bayerIndex] += tmpsrc[-1];
            rgbCounters[bayerIndex]++;
        }

        if (x < (width - 1)) {
            bayerIndex = BayerPattern[yParity * 2 + xParityInv];
            rgbValues[bayerIndex] += tmpsrc[1];
            rgbCounters[bayerIndex]++;
        }

        if (y > 0) {
            bayerIndex = BayerPattern[yParityInv * 2 + xParity];
            rgbValues[bayerIndex] += tmpsrc[-srcStride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[yParityInv * 2 + xParityInv];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[yParityInv * 2 + xParityInv];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;
            }
        }

        if (y < (height - 1)) {
            bayerIndex = BayerPattern[yParityInv * 2 + xParity];
            rgbValues[bayerIndex] += tmpsrc[srcStride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[yParityInv * 2 + xParityInv];
                rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[yParityInv * 2 + xParityInv];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;
            }
        }
    }

    tmpdst[RGB_R] = (unsigned short)(rgbValues[RGB_R] / rgbCounters[RGB_R]);
    tmpdst[RGB_G] = (unsigned short)(rgbValues[RGB_G] / rgbCounters[RGB_G]);
    tmpdst[RGB_B] = (unsigned short)(rgbValues[RGB_B] / rgbCounters[RGB_B]);
}