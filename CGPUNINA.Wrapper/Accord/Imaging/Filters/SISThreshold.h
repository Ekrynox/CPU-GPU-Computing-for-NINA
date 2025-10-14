#pragma once

#include "Accord/Imaging/Filters/SISThreshold.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	public ref class Patch_SISThreshold {
	public:
		static int32_t CalculateThreshold(UnmanagedImage^% image, System::Drawing::Rectangle% rect) {
			if (image->PixelFormat != PixelFormat::Format8bppIndexed) throw gcnew UnsupportedImageFormatException("Source pixel format is not supported by the routine.");

			pin_ptr<uint8_t> ptr = (uint8_t*)image->ImageData.ToPointer();

			return CalculateSISThreshold(ptr, rect.Left, rect.Top, rect.Width, rect.Height, image->Stride);
		}
	};

}