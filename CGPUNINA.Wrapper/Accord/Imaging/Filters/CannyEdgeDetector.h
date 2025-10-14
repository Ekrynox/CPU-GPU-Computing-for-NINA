#pragma once

#include "Accord/Imaging/Filters/CannyEdgeDetector.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	public ref class Patch_CannyEdgeDetector {
	public:
		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, System::Drawing::Rectangle% rect, ::Accord::Imaging::Filters::GaussianBlur^% gaussianFilter, System::Byte lowThreshold, System::Byte highThreshold) {
			// STEP 1 - blur image
			UnmanagedImage^ blurredImage = gaussianFilter->Apply(sourceData);

			// STEP 2 to 4
			int32_t srcWidth = sourceData->Width;
			int32_t srcHeight = sourceData->Height;

			int32_t startX = rect.Left + 1;
			int32_t startY = rect.Top + 1;
			int32_t stopX = startX + rect.Width - 2;
			int32_t stopY = startY + rect.Height - 2;

			int32_t width = rect.Width - 2;
			int32_t height = rect.Height - 2;

			int32_t dstStride = destinationData->Stride;
			int32_t srcStride = sourceData->Stride;

			int32_t dstOffset = dstStride - rect.Width + 2;
			int32_t srcOffset = srcStride - rect.Width + 2;

			pin_ptr<uint8_t> src = (uint8_t*)blurredImage->ImageData.ToPointer();
			pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

			CannyEdgeDetector(src, srcWidth, srcHeight, startX, startY, stopX, stopY, width, height, srcStride, srcOffset, dst, dstStride, dstOffset, lowThreshold, highThreshold);

			// STEP 5 - draw black rectangle to remove those pixels, which were not processed
			// (this needs to be done for those cases, when filter is applied "in place" -
			//  source image is modified instead of creating new copy)
			Drawing::Rectangle(destinationData, rect, System::Drawing::Color::Black);

			// release blurred image
			delete blurredImage;
		}
	};

}