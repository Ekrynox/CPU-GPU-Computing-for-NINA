#pragma once
#include "../../ninapp.hpp"

#include <stdint.h>



namespace LucasAlias::NINA::NinaPP::Accord::Imaging {

	NINAPP_API void mapObject8bpp(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t stride, int32_t* ObjectLabels, int32_t& labelsCount, int32_t* map, uint8_t backgroundThresholdG);
	NINAPP_API void mapObjectRGB(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t pixelSize, const int32_t stride, int32_t* ObjectLabels, int32_t& labelsCount, int32_t* map, uint8_t backgroundThresholdR, uint8_t backgroundThresholdG, uint8_t backgroundThresholdB);
	NINAPP_API void reMapObject(int32_t* ObjectLabels, const int32_t ObjectLabelsLen, const int32_t labelsCount, int32_t* map, int32_t& ObjectsCount, int32_t* reMap);

}