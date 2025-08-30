#include <algorithm>

#include "SISThreshold.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	int32_t CalculateSISThreshold(uint8_t* ptr, const int32_t startX, const int32_t startY, const int32_t width, const int32_t height, const int32_t stride) {
        int32_t stopX = startX + width;
        int32_t stopY = startY + height;
        int32_t stopXM1 = stopX - 1;
        int32_t stopYM1 = stopY - 1;
        int32_t offset = stride - width;

        // differences and weights
        double ex, ey, weight, weightTotal = 0, total = 0;

        // allign pointer to the first pixel to process
        ptr += (startY * stride + startX);

        // skip the first line for the first pass
        ptr += stride;

        // for each line
        for (int y = startY + 1; y < stopYM1; y++) {
            ptr++;
            // for each pixels
            for (int x = startX + 1; x < stopXM1; x++, ptr++) {
                // the equations are:
                // ex = | I(x + 1, y) - I(x - 1, y) |
                // ey = | I(x, y + 1) - I(x, y - 1) |
                // weight = max(ex, ey)
                // weightTotal += weight
                // total += weight * I(x, y)

                ex = std::abs(ptr[1] - ptr[-1]);
                ey = std::abs(ptr[stride] - ptr[-stride]);
                weight = (ex > ey) ? ex : ey;
                weightTotal += weight;
                total += weight * (*ptr);
            }
            ptr += offset + 1;
        }

        // calculate threshold
        return (weightTotal == 0) ? (uint8_t)0 : (uint8_t)(total / weightTotal);
	}

}