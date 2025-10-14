#pragma once
#include "../../CGPUNINA.hpp"

#include <stdint.h>
#include <vector>



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging {

	CGPUNINA_API void collectInfo8bpp(uint8_t* src, int32_t ImageWidth, int32_t ImageHeight, int32_t stride, int32_t* objectLabels, int32_t objectsCount,
        std::vector<int32_t>& x1, std::vector<int32_t>& y1, std::vector<int32_t>& x2, std::vector<int32_t>& y2,
        std::vector<int32_t>& area, std::vector<int64_t>& xc, std::vector<int64_t>& yc,
        std::vector<int64_t>& meanR, std::vector<int64_t>& meanG, std::vector<int64_t>& meanB,
        std::vector<int64_t>& stdDevR, std::vector<int64_t>& stdDevG, std::vector<int64_t>& stdDevB);

    CGPUNINA_API void collectInfoRGB(uint8_t* src, int32_t ImageWidth, int32_t ImageHeight, int32_t stride, const int32_t pixelSize, int32_t* objectLabels, int32_t objectsCount,
        std::vector<int32_t>& x1, std::vector<int32_t>& y1, std::vector<int32_t>& x2, std::vector<int32_t>& y2,
        std::vector<int32_t>& area, std::vector<int64_t>& xc, std::vector<int64_t>& yc,
        std::vector<int64_t>& meanR, std::vector<int64_t>& meanG, std::vector<int64_t>& meanB,
        std::vector<int64_t>& stdDevR, std::vector<int64_t>& stdDevG, std::vector<int64_t>& stdDevB);

}