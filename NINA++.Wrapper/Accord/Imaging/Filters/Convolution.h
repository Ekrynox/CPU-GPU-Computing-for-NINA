// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
// 
// NINA++ Plugin for NINA
// https://github.com/Ekrynox/NINAPlusPlus
// 
// Copyright © Lucas Alias, 2015
// https://github.com/Ekrynox

#pragma once

#include "Accord/Imaging/Filters/Convolution.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	public ref class Patch_Convolution {
    public:
		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, System::Drawing::Rectangle% rect, bool processAlpha, array<System::Int32, 2>^ kernel, System::Int32 divisor, System::Int32 threshold, System::Int32 size, bool dynamicDivisorForEdges, bool __MT) {
            int32_t pixelSize = System::Drawing::Image::GetPixelFormatSize(sourceData->PixelFormat) / 8;

            // processing start and stop X,Y positions
            int32_t startX = rect.Left;
            int32_t startY = rect.Top;
            int32_t stopX = startX + rect.Width;
            int32_t stopY = startY + rect.Height;

            pin_ptr<int32_t> kernel_ptr = &kernel[0, 0];

            // check pixel size to find if we deal with 8 or 16 bpp channels
            if ((pixelSize <= 4) && (pixelSize != 2)) {
                int32_t srcStride = sourceData->Stride;
                int32_t dstStride = destinationData->Stride;

                int32_t srcOffset = srcStride - rect.Width * pixelSize;
                int32_t dstOffset = dstStride - rect.Width * pixelSize;

                pin_ptr<uint8_t> src = (uint8_t*)sourceData->ImageData.ToPointer();
                pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

                // allign pointers to the first pixel to process
                src += (startY * srcStride + startX * pixelSize);
                dst += (startY * dstStride + startX * pixelSize);

                // do the processing job
                if (destinationData->PixelFormat == PixelFormat::Format8bppIndexed) {
                    // grayscale image
                    Process8bppImage(src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                }
                else {
                    // RGB image
                    if ((pixelSize == 3) || (!processAlpha)) {
                        Process24bppImage(src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY, pixelSize, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                    }
                    else {
                        Process32bppImage(src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                    }
                }
            }
            else {
                pixelSize /= 2;

                int32_t dstStride = destinationData->Stride / 2;
                int32_t srcStride = sourceData->Stride / 2;

                // base pointers
                pin_ptr<uint16_t> baseSrc = (uint16_t*)sourceData->ImageData.ToPointer();
                pin_ptr<uint16_t> baseDst = (uint16_t*)destinationData->ImageData.ToPointer();

                // allign pointers by X
                baseSrc += (startX * pixelSize);
                baseDst += (startX * pixelSize);

                if (sourceData->PixelFormat == PixelFormat::Format16bppGrayScale) {
                    // 16 bpp grayscale image
                    Process16bppImage(baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                }
                else {
                    // RGB image
                    if ((pixelSize == 3) || (!processAlpha)) {
                        Process48bppImage(baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, pixelSize, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                    }
                    else {
                        Process64bppImage(baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, kernel_ptr, divisor, threshold, size, dynamicDivisorForEdges, __MT);
                    }
                }
            }
		}
	};

}