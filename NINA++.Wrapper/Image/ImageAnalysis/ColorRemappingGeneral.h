#pragma once

using namespace NINA::Image::ImageAnalysis;
using namespace Accord::Imaging;


#include "Image/ImageAnalysis/ColorRemappingGeneral.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	public ref class Patch_ColorRemappingGeneral {
	public:
		static void ProcessFilter(UnmanagedImage^ image, System::Drawing::Rectangle^ rect, array<uint16_t>^ redMap, array<uint16_t>^ greenMap, array<uint16_t>^ blueMap, array<uint16_t>^ grayMap) {
            // processing start and stop X,Y positions
            int32_t stopX = rect->Width;
            int32_t stopY = rect->Height;

            // do the job
            pin_ptr<uint16_t> ptr = (uint16_t*)image->ImageData.ToPointer();

            if (image->PixelFormat == System::Drawing::Imaging::PixelFormat::Format16bppGrayScale) {
                pin_ptr<uint16_t> grayMap_ptr = &grayMap[0];
                colorRemapGrayscale(stopX, stopY, ptr, grayMap_ptr);
            }
            else {
                pin_ptr<uint16_t> redMap_ptr = &redMap[0];
                pin_ptr<uint16_t> greenMap_ptr = &greenMap[0];
                pin_ptr<uint16_t> blueMap_ptr = &blueMap[0];
                colorRemapRGB(stopX, stopY, ptr, redMap_ptr, greenMap_ptr, blueMap_ptr);
            }
		}
	};

}