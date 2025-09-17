#pragma once

#include "Accord/Imaging/Filters/ResizeBicubic.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	public ref class Patch_ResizeBicubic {
	public:
		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData) {
			pin_ptr<uint8_t> src = (uint8_t*)sourceData->ImageData.ToPointer();
			pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

			if (destinationData->PixelFormat == PixelFormat::Format8bppIndexed) {
				ResizeBicubicGrayScale(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
			}
			else if (sourceData->PixelSize == 3) {
				ResizeBicubicRGB(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
			}
			else if (sourceData->PixelSize == 4) {
				ResizeBicubicARGB(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
			}
			else {
				throw gcnew System::InvalidOperationException("Execution should never reach here.");
			}
		}
	};

}