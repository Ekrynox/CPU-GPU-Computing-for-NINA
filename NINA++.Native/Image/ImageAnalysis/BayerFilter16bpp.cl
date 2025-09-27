#define RGB_R 2
#define RGB_G 1
#define RGB_B 0

__kernel void debayerPattern(const int width, const int height, const int LOCAL_X, const int LOCAL_Y, __global unsigned short* src, __global unsigned short* dst, __local unsigned short* tile, const int srcStride, const int srcOffset, const int dstOffset, __constant int* BayerPattern, __global unsigned short* Rarr, __global unsigned short* Garr, __global unsigned short* Barr, __global unsigned short* Larr) {
	int y = get_global_id(0);
	int x = get_global_id(1);

    if (y >= height || x >= width) return;

	int ly = get_local_id(0);
	int lx = get_local_id(1);

	int lstride = LOCAL_X + 2;

	__global unsigned short* tmpsrc = src + y * srcStride + x;
    __global unsigned short* tmpdst = dst + y * (3 * width + dstOffset) + x * 3;
	unsigned int counter = y * width + x;

	tile[(ly+1) * lstride + (lx + 1)] = tmpsrc[0];
	if (lx == 0 && x > 0) {
		tile[(ly + 1) * lstride] = tmpsrc[-1];
	}
	if (lx + 1 == LOCAL_X && x < width - 1) {
		tile[(ly + 1) * lstride + (lx + 2)] = tmpsrc[1];
	}

	if (ly == 0 && y > 0) {
		tile[(lx + 1)] = tmpsrc[-srcStride];

		if (lx == 0 && x > 0) {
			tile[0] = tmpsrc[-srcStride-1];
		}
		if (lx + 1 == LOCAL_X && x < width - 1) {
			tile[(lx + 2)] = tmpsrc[-srcStride+1];
		}
	}
	if (ly + 1 == LOCAL_Y && y < height - 1) {
		tile[(ly + 2) * lstride + (lx + 1)] = tmpsrc[srcStride];

		if (lx == 0 && x > 0) {
			tile[(ly + 2) * lstride] = tmpsrc[srcStride-1];
		}
		if (lx + 1 == LOCAL_X && x < width - 1) {
			tile[(ly + 2) * lstride + (lx + 2)] = tmpsrc[srcStride+1];
		}
	}

	barrier(CLK_LOCAL_MEM_FENCE);

    // Debayering
    unsigned int rgbValues[3];
    unsigned int rgbCounters[3];
    unsigned int bayerIndex;

    rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
    rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

    __local unsigned short* tmptile = tile + (ly + 1) * lstride + (lx + 1);

    if ((y > 0) && (y < (height - 1)) && (x > 0) && (x < (width - 1))) {
        bayerIndex = BayerPattern[(y & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += *tmptile;
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[(y & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmptile[-1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[(y & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmptile[1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += tmptile[-lstride];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmptile[-lstride - 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmptile[-lstride + 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += tmptile[lstride];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x - 1) & 1)];
        rgbValues[bayerIndex] += tmptile[lstride - 1];
        rgbCounters[bayerIndex]++;

        bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x + 1) & 1)];
        rgbValues[bayerIndex] += tmptile[lstride + 1];
        rgbCounters[bayerIndex]++;
    }
    else {
        bayerIndex = BayerPattern[(y & 1) * 2 + (x & 1)];
        rgbValues[bayerIndex] += *tmptile;
        rgbCounters[bayerIndex]++;

        if (x > 0) {
            bayerIndex = BayerPattern[(y & 1) * 2 + ((x - 1) & 1)];
            rgbValues[bayerIndex] += tmptile[-1];
            rgbCounters[bayerIndex]++;
        }

        if (x < (width - 1)) {
            bayerIndex = BayerPattern[(y & 1) * 2 + ((x + 1) & 1)];
            rgbValues[bayerIndex] += tmptile[1];
            rgbCounters[bayerIndex]++;
        }

        if (y > 0) {
            bayerIndex = BayerPattern[((y - 1) & 1) * 2 + (x & 1)];
            rgbValues[bayerIndex] += tmptile[-lstride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmptile[-lstride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[((y - 1) & 1) * 2 + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmptile[-lstride + 1];
                rgbCounters[bayerIndex]++;
            }
        }

        if (y < (height - 1)) {
            bayerIndex = BayerPattern[((y + 1) & 1) * 2 + (x & 1)];
            rgbValues[bayerIndex] += tmptile[lstride];
            rgbCounters[bayerIndex]++;

            if (x > 0) {
                bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmptile[lstride - 1];
                rgbCounters[bayerIndex]++;
            }

            if (x < (width - 1)) {
                bayerIndex = BayerPattern[((y + 1) & 1) * 2 + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmptile[lstride + 1];
                rgbCounters[bayerIndex]++;
            }
        }
    }

    rgbValues[RGB_R] = (unsigned short)(rgbValues[RGB_R] / rgbCounters[RGB_R]);
    rgbValues[RGB_G] = (unsigned short)(rgbValues[RGB_G] / rgbCounters[RGB_G]);
    rgbValues[RGB_B] = (unsigned short)(rgbValues[RGB_B] / rgbCounters[RGB_B]);

    tmpdst[RGB_R] = (unsigned short)rgbValues[RGB_R];
    tmpdst[RGB_G] = (unsigned short)rgbValues[RGB_G];
    tmpdst[RGB_B] = (unsigned short)rgbValues[RGB_B];

    Rarr[counter] = (unsigned short)rgbValues[RGB_B];
    Garr[counter] = (unsigned short)rgbValues[RGB_G];
    Barr[counter] = (unsigned short)rgbValues[RGB_R];
    Larr[counter] = (unsigned short)((rgbValues[RGB_R] + rgbValues[RGB_G] + rgbValues[RGB_B]) / 3u);
}