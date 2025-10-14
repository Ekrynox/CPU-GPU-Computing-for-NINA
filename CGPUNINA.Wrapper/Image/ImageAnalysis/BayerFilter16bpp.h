/*
	Copyright © 2016 - 2024 Stefan Berg <isbeorn86+NINA@googlemail.com> and the N.I.N.A. contributors
	Copyright © 2025 - Lucas Alias <https://github.com/Ekrynox> (adapted to C++)

	This file is part of N.I.N.A. - Nighttime Imaging 'N' Astronomy.

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.

	Notes:
	- This file is a modified/adapted version of the original N.I.N.A. C# code.
	- Modifications include translation to C++.
*/

#pragma once

using namespace NINA::Image::ImageAnalysis;
using namespace NINA::Image::ImageData;
using namespace Accord::Imaging;
using namespace Accord::Imaging::Filters;


#include "Image/ImageAnalysis/BayerFilter16bpp.hpp"
#include "NinaCL.h"




namespace LucasAlias::NINA::CGPUNINA::Image::ImageAnalysis {

	public ref class Patch_BayerFilter16bpp {
	public:

		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, ::NINA::Image::ImageData::LRGBArrays^% LRGBArrays, array<int, 2>^ BayerPattern, bool^ SaveColorChannels, bool^ SaveLumChannel, bool^ PerformDemosaicing, bool^ __MT) {
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
			int32_t srcOffset = (srcStride - width) / 2;
			int32_t dstOffset = (destinationData->Stride - width * 6) / 6;

			pin_ptr<int32_t> bayerPattern = &BayerPattern[0, 0];

			if (!PerformDemosaicing) {
				copyImage(width, height, src, dst, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1));
			}
			else {
				if (SaveColorChannels && SaveLumChannel) {
					debayerPattern(width, height, src, dst, srcStride, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1), Rarr, Garr, Barr, Larr, *__MT);
				}
				else if (!SaveColorChannels && SaveLumChannel) {
					debayerPatternL(width, height, src, dst, srcStride, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1), Larr, *__MT);
				}
				else if (SaveColorChannels && !SaveLumChannel) {
					debayerPatternRGB(width, height, src, dst, srcStride, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1), Rarr, Garr, Barr, *__MT);
				}
			}
		}


		static void ProcessFilterOpenCL(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, ::NINA::Image::ImageData::LRGBArrays^% LRGBArrays, array<int, 2>^ BayerPattern, bool^ SaveColorChannels, bool^ SaveLumChannel, bool^ PerformDemosaicing, bool^ __MT, OpenCL::Manager^ OpCLM, System::UInt32 context) {
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
			int32_t srcOffset = (srcStride - width) / 2;
			int32_t dstOffset = (destinationData->Stride - width * 6) / 6;

			pin_ptr<int32_t> bayerPattern = &BayerPattern[0, 0];

			if (!PerformDemosaicing) {
				copyImage(width, height, src, dst, srcOffset, dstOffset, bayerPattern, BayerPattern->GetLength(1));
			}
			else {
				debayerPatternOpenCL(OpCLM->GetNative(), context, width, height, src, dst, srcStride, srcOffset, dstOffset, bayerPattern);

				if (SaveColorChannels && SaveLumChannel) {
					rgblArrCopy(width, height, dst, dstOffset, Rarr, Garr, Barr, Larr, *__MT);
				}
				else if (!SaveColorChannels && SaveLumChannel) {
					rgbArrCopy(width, height, dst, dstOffset, Rarr, Garr, Barr, *__MT);
				}
				else if (SaveColorChannels && !SaveLumChannel) {
					lArrCopy(width, height, dst, dstOffset, Larr, *__MT);
				}
			}
		}
	};

}