#pragma once
#include "../../CGPUNINA.hpp"

#include <math.h>
#include <stdint.h>
#include <vector>

#include "../struct.hpp"



namespace LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis::CPPStarDetection {

	struct PixelData_ {
		int32_t PosX;
		int32_t PosY;
		int32_t Value;
	};
	typedef struct PixelData_ PixelData;

	namespace CPPStar {
		CGPUNINA_API void Calculate(const std::vector<PixelData>& pixelData, CppPoint& _Position, const CppRectangle& _Rectangle, double& _Average, double& _HFR, const double _Radius, const double _SurroundingMean);
	}
}