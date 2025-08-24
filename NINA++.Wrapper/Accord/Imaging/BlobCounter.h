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

#include <numeric>

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
            pin_ptr<int32_t> ObjectLabels_ptr = &ObjectLabels[0];
            pin_ptr<int32_t> map_ptr = &map[0];

            if (image->PixelFormat == PixelFormat::Format8bppIndexed) {
                mapObject8bpp(src, ImageWidth, ImageHeight, stride, ObjectLabels_ptr, labelsCount, map_ptr, backgroundThresholdG);
            }
            else {
                int pixelSize = Bitmap::GetPixelFormatSize(image->PixelFormat) / 8;
                mapObjectRGB(src, ImageWidth, ImageHeight, pixelSize, stride, ObjectLabels_ptr, labelsCount, map_ptr, backgroundThresholdR, backgroundThresholdG, backgroundThresholdB);
            }

            // allocate remapping array
            array<int>^ reMap = gcnew array<int>(map->Length);
            pin_ptr<int32_t> reMap_ptr = &reMap[0];

            // count objects and prepare remapping array
            auto ObjectsCount_n = ObjectsCount;
            reMapObject(ObjectLabels_ptr, ObjectLabels->Length, labelsCount, map_ptr, ObjectsCount_n, reMap_ptr);
            ObjectsCount = ObjectsCount_n;
		}

	};

}