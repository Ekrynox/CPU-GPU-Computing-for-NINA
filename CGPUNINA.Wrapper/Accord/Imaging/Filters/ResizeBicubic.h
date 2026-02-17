#pragma once

#include "Accord/Imaging/Filters/ResizeBicubic.hpp"

#include "NinaCL.h"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging::Filters {

	public ref class Patch_ResizeBicubic {
	public:

		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, bool __MT) {
			pin_ptr<uint8_t> src = (uint8_t*)sourceData->ImageData.ToPointer();
			pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

			if (destinationData->PixelFormat == PixelFormat::Format8bppIndexed) {
				ResizeBicubicGrayScale(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset, __MT);
			}
			else if (sourceData->PixelSize == 3) {
				ResizeBicubicRGB(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset, __MT);
			}
			else if (sourceData->PixelSize == 4) {
				ResizeBicubicARGB(src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset, __MT);
			}
			else {
				throw gcnew System::InvalidOperationException("Execution should never reach here.");
			}
		}

		static void ProcessFilterOpenCL(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, OpenCL::Manager^ OpCLM, System::UInt32 context) {
			pin_ptr<uint8_t> src = (uint8_t*)sourceData->ImageData.ToPointer();
			pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

			try {
				if (destinationData->PixelFormat == PixelFormat::Format8bppIndexed) {
					ResizeBicubicGrayScaleOpenCL(OpCLM->GetNative(), context, src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
				}
				else if (sourceData->PixelSize == 3) {
					ResizeBicubicRGBOpenCL(OpCLM->GetNative(), context, src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
				}
				else if (sourceData->PixelSize == 4) {
					ResizeBicubicARGBOpenCL(OpCLM->GetNative(), context, src, sourceData->Width, sourceData->Height, sourceData->Stride, dst, destinationData->Width, destinationData->Height, destinationData->Stride, destinationData->Offset);
				}
				else {
					throw gcnew System::InvalidOperationException("Execution should never reach here.");
				}
			}
			catch (const std::exception& e) {
				System::String^ msg = gcnew System::String(e.what());
				throw gcnew System::Exception(msg);
			}
		}

	};

}