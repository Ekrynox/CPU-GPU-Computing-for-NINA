#pragma once
#include "../../../CGPUNINA.hpp"

#include <stdint.h>

#include "../../../ninacl.hpp"
#include "../../../Image/enums.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	template <typename T> CGPUNINA_API void Process1CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	template <typename T> CGPUNINA_API void Process3CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t pixelSize, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);
	template <typename T> CGPUNINA_API void Process4CImage(T* baseSrc, T* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernel, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges, const bool __MT);


	CGPUNINA_API void Process1CImage8bppOpenCL(OpenCLManager& opCLM, size_t context, uint8_t* baseSrc, uint8_t* baseDst, int32_t srcStride, int32_t dstStride, int32_t startX, int32_t startY, int32_t stopX, int32_t stopY, int32_t* kernelPattern, int32_t divisor, int32_t threshold, int32_t size, bool dynamicDivisorForEdges);
}