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
#include <math.h>

#include "Accord/Imaging/BlobCounterBase.hpp"



namespace LucasAlias::NINA::CGPUNINA::Accord::Imaging {

	public ref class Patch_BlobCounterBases {
	public:

        static void CollectObjectsInfo(UnmanagedImage^% image, const System::Int32 ImageWidth, const System::Int32 ImageHeight, array<System::Int32>^% objectLabels, const System::Int32 objectsCount, System::Collections::Generic::List<Blob^>^% blobs) {
            int i = 0, label;

            // create object coordinates arrays
            auto x1 = std::vector<int32_t>(objectsCount + 1);
            auto y1 = std::vector<int32_t>(objectsCount + 1);
            auto x2 = std::vector<int32_t>(objectsCount + 1);
            auto y2 = std::vector<int32_t>(objectsCount + 1);

            auto area = std::vector<int32_t>(objectsCount + 1);
            auto xc = std::vector<int64_t>(objectsCount + 1);
            auto yc = std::vector<int64_t>(objectsCount + 1);

            auto meanR = std::vector<int64_t>(objectsCount + 1);
            auto meanG = std::vector<int64_t>(objectsCount + 1);
            auto meanB = std::vector<int64_t>(objectsCount + 1);

            auto stdDevR = std::vector<int64_t>(objectsCount + 1);
            auto stdDevG = std::vector<int64_t>(objectsCount + 1);
            auto stdDevB = std::vector<int64_t>(objectsCount + 1);

            pin_ptr<uint8_t> src = (uint8_t*)image->ImageData.ToPointer();
            pin_ptr<int32_t> objectLabels_ptr = &objectLabels[0];

            if (image->PixelFormat == PixelFormat::Format8bppIndexed) {
                collectInfo8bpp(src, ImageWidth, ImageHeight, image->Stride, objectLabels_ptr, objectsCount, x1, y1, x2, y2, area, xc, yc, meanR, meanG, meanB, stdDevR, stdDevG, stdDevB);
            }
            else {
                System::Int32 pixelSize = Bitmap::GetPixelFormatSize(image->PixelFormat) / 8;
                collectInfoRGB(src, ImageWidth, ImageHeight, image->Stride, pixelSize, objectLabels_ptr, objectsCount, x1, y1, x2, y2, area, xc, yc, meanR, meanG, meanB, stdDevR, stdDevG, stdDevB);
            }

            // create blobs
            blobs->Clear();

            for (System::Int32 j = 1; j <= objectsCount; j++)
            {
                System::Int32 blobArea = area[j];

                Blob^ blob = gcnew Blob(j, System::Drawing::Rectangle(x1[j], y1[j], x2[j] - x1[j], y2[j] - y1[j]));
                blob->Area = blobArea;
                blob->Fullness = (double)blobArea / ((x2[j] - x1[j] + 1) * (y2[j] - y1[j] + 1));
                blob->CenterOfGravity = ::Accord::Point((float)xc[j] / blobArea, (float)yc[j] / blobArea);
                blob->ColorMean = Color::FromArgb((uint8_t)(meanR[j] / blobArea), (uint8_t)(meanG[j] / blobArea), (uint8_t)(meanB[j] / blobArea));
                blob->ColorStdDev = Color::FromArgb(
                    (uint8_t)(sqrt(stdDevR[j] / blobArea - blob->ColorMean.R * blob->ColorMean.R)),
                    (uint8_t)(sqrt(stdDevG[j] / blobArea - blob->ColorMean.G * blob->ColorMean.G)),
                    (uint8_t)(sqrt(stdDevB[j] / blobArea - blob->ColorMean.B * blob->ColorMean.B)));

                blobs->Add(blob);
            }
        }

	};

}