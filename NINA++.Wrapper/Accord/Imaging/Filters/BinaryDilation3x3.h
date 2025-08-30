// Accord Imaging Library
// The Accord.NET Framework
// http://accord-framework.net
//
// Copyright © César Souza, 2009-2017
// cesarsouza at gmail.com
//
// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2005-2010
// contacts@aforgenet.com
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

#include "Accord/Imaging/Filters/BinaryDilation3x3.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging::Filters {

	public ref class Patch_BinaryDilation3x3 {
	public:
		static void ProcessFilter(UnmanagedImage^% sourceData, UnmanagedImage^% destinationData, System::Drawing::Rectangle% rect) {
			if ((rect.Width < 3) || (rect.Height < 3)) throw gcnew InvalidImagePropertiesException("Processing rectangle mast be at least 3x3 in size.");

			pin_ptr<uint8_t> src = (uint8_t*)sourceData->ImageData.ToPointer();
			pin_ptr<uint8_t> dst = (uint8_t*)destinationData->ImageData.ToPointer();

			int32_t startX = rect.Left + 1;
			int32_t startY = rect.Top + 1;
			int32_t stopX = rect.Right - 1;
			int32_t stopY = rect.Bottom - 1;

			int32_t dstStride = destinationData->Stride;
			int32_t srcStride = sourceData->Stride;

			int32_t dstOffset = dstStride - rect.Width + 1;
			int32_t srcOffset = srcStride - rect.Width + 1;

			BinaryDilation3x3(src, startX, startY, stopX, stopY, srcStride, srcOffset, dst, dstStride, dstOffset);
		}
	};

}