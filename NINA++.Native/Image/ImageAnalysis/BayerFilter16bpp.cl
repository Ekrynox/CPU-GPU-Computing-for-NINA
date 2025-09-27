#define RGB_R 2
#define RGB_G 1
#define RGB_B 0

__kernel void debayerPattern(const int width, const int height, __global unsigned short* src, __global unsigned short* dst, const int srcStride, const int srcOffset, const int dstOffset, __constant int* BayerPattern, __global unsigned short* Rarr, __global unsigned short* Garr, __global unsigned short* Barr, __global unsigned short* Larr) {
	int y = get_global_id(0);
	int x = get_global_id(1);

    if (y < 0 || y >= height || x < 0 || x >= width) return;

    unsigned int rgbValues[3];
    unsigned int rgbCounters[3];
    unsigned int bayerIndex;

    rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
    rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

    unsigned short* tmpsrc = src + y * srcStride + x;
    unsigned short* tmpdst = dst + y * (3 * width + dstOffset) + x * 3;
    unsigned int counter = y * width + x;

	if ((y > 0) && (y < (height - 1)) && (x > 0) && (x < (width - 1))) {
        bayerIndex = BayerPattern[(y & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += *tmpsrc;
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[(y & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[-1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[(y & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += tmpsrc[-srcStride];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += tmpsrc[srcStride];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
        rgbCounters[bayerIndex]++;
	}
    else {
        bayerIndex = BayerPattern[(y & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += *tmpsrc;
        rgbCounters[bayerIndex]++;

        if (x > 0) {
            bayerIndex = BayerPattern[(y & 1) * 2 + ((x - 1) & 1)];
            rgbValues[bayerIndex] += tmpsrc[-1];
            rgbCounters[bayerIndex]++;
        }

        if (x < (width - 1)) {
            bayerIndex = BayerPattern[(y & 1) * 2 + ((x + 1) & 1)];
            rgbValues[bayerIndex] += tmpsrc[1];
            rgbCounters[bayerIndex]++;
        }

        if (y > 0) {
            bayerIndex = BayerPattern[((y - 1) & 1) * 2 + (x & 1)];
            rgbValues[bayerIndex] += tmpsrc[-srcStride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;
            }
        }

        if (y < (height - 1)) {
            bayerIndex = BayerPattern[((y + 1) & 1) * 2 + (x & 1)];
            rgbValues[bayerIndex] += tmpsrc[srcStride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;
            }
        }
    }

    tmpdst[RGB_R] = (unsigned short)(rgbValues[RGB_R] / rgbCounters[RGB_R]);
    tmpdst[RGB_G] = (unsigned short)(rgbValues[RGB_G] / rgbCounters[RGB_G]);
    tmpdst[RGB_B] = (unsigned short)(rgbValues[RGB_B] / rgbCounters[RGB_B]);

    Rarr[counter] = tmpdst[RGB_B];
    Garr[counter] = tmpdst[RGB_G];
    Barr[counter] = tmpdst[RGB_R];
    Larr[counter] = (unsigned short)floor(((unsigned int)tmpdst[RGB_R] + (unsigned int)tmpdst[RGB_G] + (unsigned int)tmpdst[RGB_B]) / 3.0);
}