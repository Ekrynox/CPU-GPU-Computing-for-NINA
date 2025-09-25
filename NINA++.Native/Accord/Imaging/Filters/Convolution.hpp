#pragma once
#include "../../../ninapp.hpp"

#include <stdint.h>

#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	template <typename T> NINAPP_API void Process1CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	extern template NINAPP_API void Process1CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
	extern template NINAPP_API void Process1CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);

	template <typename T> NINAPP_API void Process3CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
    extern template NINAPP_API void Process3CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
    extern template NINAPP_API void Process3CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
	
	template <typename T> NINAPP_API void Process4CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	template NINAPP_API void Process4CImage<uint8_t>(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
	template NINAPP_API void Process4CImage<uint16_t>(uint16_t*, uint16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t*, int32_t, int32_t, int32_t, bool, const bool);
}