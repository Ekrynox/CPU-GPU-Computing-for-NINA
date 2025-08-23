#pragma once

using namespace NINA::Image::ImageAnalysis;
using namespace NINA::Image::ImageData;
using namespace Accord::Imaging;
using namespace Accord::Imaging::Filters;


#include "Image/ImageAnalysis/BayerFilter16bpp.hpp"




namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	public ref class Patch_BayerFilter16bpp {
	public:

		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, ::NINA::Image::ImageData::LRGBArrays^% LRGBArrays, array<int, 2>^ BayerPattern, bool^ SaveColorChannels, bool^ SaveLumChannel, bool^ PerformDemosaicing) {
			int32_t width = sourceData->Width;
			int32_t height = sourceData->Height;


			pin_ptr<uint16_t> Rarr = nullptr;
			pin_ptr<uint16_t> Garr = nullptr;
			pin_ptr<uint16_t> Barr = nullptr;
			pin_ptr<uint16_t> Larr = nullptr;
			if (SaveColorChannels && SaveLumChannel) {
				LRGBArrays = gcnew ::NINA::Image::ImageData::LRGBArrays(gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(width * height));
				Rarr = &LRGBArrays->Red[0];
				Garr = &LRGBArrays->Green[0];
				Barr = &LRGBArrays->Blue[0];
				Larr = &LRGBArrays->Lum[0];
			}
			else if (!SaveColorChannels && SaveLumChannel) {
				LRGBArrays = gcnew ::NINA::Image::ImageData::LRGBArrays(gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(0), gcnew array<System::UInt16>(0), gcnew array<System::UInt16>(0));
				Larr = &LRGBArrays->Lum[0];
			}
			else if (SaveColorChannels && !SaveLumChannel) {
				LRGBArrays = gcnew ::NINA::Image::ImageData::LRGBArrays(gcnew array<System::UInt16>(0), gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(width * height), gcnew array<System::UInt16>(width * height));
				Rarr = &LRGBArrays->Red[0];
				Garr = &LRGBArrays->Green[0];
				Barr = &LRGBArrays->Blue[0];
			}


			pin_ptr<uint16_t> src = (uint16_t*)sourceData->ImageData.ToPointer();
			pin_ptr<uint16_t> dst = (uint16_t*)destinationData->ImageData.ToPointer();

			int32_t srcStride = sourceData->Stride / 2;
			int32_t dstStride = destinationData->Stride;

			int32_t srcOffset = (srcStride - width) / 2;
			int32_t dstOffset = (dstStride - width * 6) / 6;

			pin_ptr<int32_t> bayerPattern = &BayerPattern[0, 0];

			if (!PerformDemosaicing) {
				// for each line
				for (int32_t y = 0; y < height; y++) {
					// for each pixel
					for (int32_t x = 0; x < width; x++, src++, dst += 3) {
						dst[::RGB::R] = 0;
						dst[::RGB::G] = 0;
						dst[::RGB::B] = 0;
						dst[BayerPattern[y & 1, x & 1]] = *src;
					}
					src += srcOffset;
					dst += dstOffset;
				}
			}
			else {
				debayerPattern(width, height, src, dst, srcStride, dstStride, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1), Rarr, Garr, Barr, Larr);
			}
		}

	};

}