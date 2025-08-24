// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
//
// Accord Imaging Library
// The Accord.NET Framework
// http://accord-framework.net
//
// Copyright © César Souza, 2009-2017
// cesarsouza at gmail.com
// 
// NINA++ Plugin for NINA
// https://github.com/Ekrynox/NINAPlusPlus
// 
// Copyright © Lucas Alias, 2015
// https://github.com/Ekrynox
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#pragma once

using namespace Accord::Imaging;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

#include "Accord/Imaging/BlobCounter.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging {

	public ref class Patch_BlobCounter {
	public:

		static void BuildObjectsMap(UnmanagedImage^% image, int ImageWidth, int ImageHeight, array<int>^% ObjectLabels, int% ObjectsCount, System::Byte backgroundThresholdR, System::Byte backgroundThresholdG, System::Byte backgroundThresholdB) {
            int stride = image->Stride;

            // check pixel format
            if ((image->PixelFormat != PixelFormat::Format8bppIndexed) &&
                (image->PixelFormat != PixelFormat::Format24bppRgb) &&
                (image->PixelFormat != PixelFormat::Format32bppRgb) &&
                (image->PixelFormat != PixelFormat::Format32bppArgb) &&
                (image->PixelFormat != PixelFormat::Format32bppPArgb)) throw gcnew UnsupportedImageFormatException("Unsupported pixel format of the source image.");

            // we don't want one pixel width images
            if (ImageWidth == 1) throw gcnew InvalidImagePropertiesException("BlobCounter cannot process images that are one pixel wide. Rotate the image or use RecursiveBlobCounter.");
            int imageWidthM1 = ImageWidth - 1;

            // allocate labels array
            ObjectLabels = gcnew array<int>(ImageWidth * ImageHeight);
            int labelsCount = 0;

            // create map
            int maxObjects = ((ImageWidth / 2) + 1) * ((ImageHeight / 2) + 1) + 1;
            array<int>^ map = gcnew array<int>(maxObjects);
            for (int i = 0; i < maxObjects; i++) map[i] = i;

            // do the job
            uint8_t* src = (uint8_t*)image->ImageData.ToPointer();
            int p = 0;

            if (image->PixelFormat == PixelFormat::Format8bppIndexed) {
                int offset = stride - ImageWidth;

                // 1 - for pixels of the first row
                if (*src > backgroundThresholdG) ObjectLabels[p] = ++labelsCount;
                ++src;
                ++p;

                // process the rest of the first row
                for (int x = 1; x < ImageWidth; x++, src++, p++) {
                    // check if we need to label current pixel
                    if (*src > backgroundThresholdG) {

                        // check if the previous pixel already was labeled
                        if (src[-1] > backgroundThresholdG) {
                            // label current pixel, as the previous
                            ObjectLabels[p] = ObjectLabels[p - 1];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                }
                src += offset;

                // 2 - for other rows
                // for each row
                for (int y = 1; y < ImageHeight; y++) {
                    // for the first pixel of the row, we need to check
                    // only upper and upper-right pixels

                    if (*src > backgroundThresholdG) {
                        // check surrounding pixels
                        if (src[-stride] > backgroundThresholdG) {
                            // label current pixel, as the above
                            ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                        }
                        else if (src[1 - stride] > backgroundThresholdG) {
                            // label current pixel, as the above right
                            ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                    ++src;
                    ++p;

                    // check left pixel and three upper pixels for the rest of pixels
                    for (int x = 1; x < imageWidthM1; x++, src++, p++) {
                        if (*src > backgroundThresholdG) {
                            // check surrounding pixels
                            if (src[-1] > backgroundThresholdG) {
                                // label current pixel, as the left
                                ObjectLabels[p] = ObjectLabels[p - 1];
                            }
                            else if (src[-1 - stride] > backgroundThresholdG) {
                                // label current pixel, as the above left
                                ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                            }
                            else if (src[-stride] > backgroundThresholdG) {
                                // label current pixel, as the above
                                ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                            }

                            if (src[1 - stride] > backgroundThresholdG) {
                                if (ObjectLabels[p] == 0) {
                                    // label current pixel, as the above right
                                    ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                                }
                                else {
                                    int l1 = ObjectLabels[p];
                                    int l2 = ObjectLabels[p + 1 - ImageWidth];

                                    if ((l1 != l2) && (map[l1] != map[l2])) {
                                        // merge
                                        if (map[l1] == l1) {
                                            // map left value to the right
                                            map[l1] = map[l2];
                                        }
                                        else if (map[l2] == l2) {
                                            // map right value to the left
                                            map[l2] = map[l1];
                                        }
                                        else {
                                            // both values already mapped
                                            map[map[l1]] = map[l2];
                                            map[l1] = map[l2];
                                        }

                                        // reindex
                                        for (int i = 1; i <= labelsCount; i++) {
                                            if (map[i] != i) {
                                                // reindex
                                                int j = map[i];
                                                while (j != map[j])
                                                {
                                                    j = map[j];
                                                }
                                                map[i] = j;
                                            }
                                        }
                                    }
                                }
                            }

                            // label the object if it is not yet
                            if (ObjectLabels[p] == 0) {
                                // create new label
                                ObjectLabels[p] = ++labelsCount;
                            }
                        }
                    }

                    // for the last pixel of the row, we need to check
                    // only upper and upper-left pixels
                    if (*src > backgroundThresholdG)
                    {
                        // check surrounding pixels
                        if (src[-1] > backgroundThresholdG) {
                            // label current pixel, as the left
                            ObjectLabels[p] = ObjectLabels[p - 1];
                        }
                        else if (src[-1 - stride] > backgroundThresholdG) {
                            // label current pixel, as the above left
                            ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                        }
                        else if (src[-stride] > backgroundThresholdG) {
                            // label current pixel, as the above
                            ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                    ++src;
                    ++p;

                    src += offset;
                }
            }
            else {
                // color images
                int pixelSize = Bitmap::GetPixelFormatSize(image->PixelFormat) / 8;
                int offset = stride - ImageWidth * pixelSize;

                int strideM1 = stride - pixelSize;
                int strideP1 = stride + pixelSize;

                // 1 - for pixels of the first row
                if ((src[::RGB::R] | src[::RGB::G] | src[::RGB::B]) != 0) ObjectLabels[p] = ++labelsCount;
                src += pixelSize;
                ++p;

                // process the rest of the first row
                for (int x = 1; x < ImageWidth; x++, src += pixelSize, p++) {
                    // check if we need to label current pixel
                    if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                        // check if the previous pixel already was labeled
                        if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                            // label current pixel, as the previous
                            ObjectLabels[p] = ObjectLabels[p - 1];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                }
                src += offset;

                // 2 - for other rows
                // for each row
                for (int y = 1; y < ImageHeight; y++) {
                    // for the first pixel of the row, we need to check
                    // only upper and upper-right pixels
                    if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                        // check surrounding pixels
                        if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                            // label current pixel, as the above
                            ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                        }
                        else if ((src[::RGB::R - strideM1] > backgroundThresholdR) || (src[::RGB::G - strideM1] > backgroundThresholdG) || (src[::RGB::B - strideM1] > backgroundThresholdB)) {
                            // label current pixel, as the above right
                            ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                    src += pixelSize;
                    ++p;

                    // check left pixel and three upper pixels for the rest of pixels
                    for (int x = 1; x < ImageWidth - 1; x++, src += pixelSize, p++) {
                        if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                            // check surrounding pixels
                            if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                                // label current pixel, as the left
                                ObjectLabels[p] = ObjectLabels[p - 1];
                            }
                            else if ((src[::RGB::R - strideP1] > backgroundThresholdR) || (src[::RGB::G - strideP1] > backgroundThresholdG) || (src[::RGB::B - strideP1] > backgroundThresholdB)) {
                                // label current pixel, as the above left
                                ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                            }
                            else if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                                // label current pixel, as the above
                                ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                            }

                            if ((src[::RGB::R - strideM1] > backgroundThresholdR) || (src[::RGB::G - strideM1] > backgroundThresholdG) || (src[::RGB::B - strideM1] > backgroundThresholdB)) {
                                if (ObjectLabels[p] == 0) {
                                    // label current pixel, as the above right
                                    ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                                }
                                else {
                                    int l1 = ObjectLabels[p];
                                    int l2 = ObjectLabels[p + 1 - ImageWidth];

                                    if ((l1 != l2) && (map[l1] != map[l2])) {
                                        // merge
                                        if (map[l1] == l1) {
                                            // map left value to the right
                                            map[l1] = map[l2];
                                        }
                                        else if (map[l2] == l2) {
                                            // map right value to the left
                                            map[l2] = map[l1];
                                        }
                                        else {
                                            // both values already mapped
                                            map[map[l1]] = map[l2];
                                            map[l1] = map[l2];
                                        }

                                        // reindex
                                        for (int i = 1; i <= labelsCount; i++) {
                                            if (map[i] != i) {
                                                // reindex
                                                int j = map[i];
                                                while (j != map[j]) {
                                                    j = map[j];
                                                }
                                                map[i] = j;
                                            }
                                        }
                                    }
                                }
                            }

                            // label the object if it is not yet
                            if (ObjectLabels[p] == 0) {
                                // create new label
                                ObjectLabels[p] = ++labelsCount;
                            }
                        }
                    }

                    // for the last pixel of the row, we need to check
                    // only upper and upper-left pixels
                    if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                        // check surrounding pixels
                        if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                            // label current pixel, as the left
                            ObjectLabels[p] = ObjectLabels[p - 1];
                        }
                        else if ((src[::RGB::R - strideP1] > backgroundThresholdR) || (src[::RGB::G - strideP1] > backgroundThresholdG) || (src[::RGB::B - strideP1] > backgroundThresholdB)) {
                            // label current pixel, as the above left
                            ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                        }
                        else if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                            // label current pixel, as the above
                            ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                        }
                        else {
                            // create new label
                            ObjectLabels[p] = ++labelsCount;
                        }
                    }
                    src += pixelSize;
                    ++p;

                    src += offset;
                }
            }

            // allocate remapping array
            array<int>^ reMap = gcnew array<int>(map->Length);

            // count objects and prepare remapping array
            ObjectsCount = 0;
            for (int i = 1; i <= labelsCount; i++) {
                if (map[i] == i) {
                    // increase objects count
                    reMap[i] = ++ObjectsCount;
                }
            }
            // second pass to complete remapping
            for (int i = 1; i <= labelsCount; i++)
            {
                if (map[i] != i) reMap[i] = reMap[map[i]];
            }

            // repair object labels
            for (int i = 0, n = ObjectLabels->Length; i < n; i++) {
                ObjectLabels[i] = reMap[ObjectLabels[i]];
            }
		}

	};

}