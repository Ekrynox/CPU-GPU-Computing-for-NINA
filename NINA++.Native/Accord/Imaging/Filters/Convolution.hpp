#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>

#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	NINAPP_API void Process8bppImage(uint8_t* src, uint8_t* dst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	NINAPP_API void Process24bppImage(uint8_t* src, uint8_t* dst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	NINAPP_API void Process32bppImage(uint8_t* src, uint8_t* dst, int32_t srcStride, int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	NINAPP_API void Process16bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	NINAPP_API void Process48bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	NINAPP_API void Process64bppImage(uint16_t* baseSrc, uint16_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);

}