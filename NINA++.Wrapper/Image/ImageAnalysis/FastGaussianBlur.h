#pragma once

#include "Image/ImageAnalysis/FastGaussianBlur.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	public ref class Patch_FastGaussianBlur {
    public:
        static void gaussBlur_4(array<System::Byte>^ source, array<System::Byte>^ dest, System::Int32 r, System::Int32 _width, System::Int32 _height) {
            pin_ptr<uint8_t> src = &source[0];
            pin_ptr<uint8_t> dst = &dest[0];
            LucasAlias::NINA::NinaPP::Image::ImageAnalysis::gaussBlur_4(src, source->Length, dst, r, _width, _height);
        }

        static array<System::Int32>^ boxesForGauss(System::Int32 sigma, System::Int32 n) {
            auto res = LucasAlias::NINA::NinaPP::Image::ImageAnalysis::boxesForGauss(sigma, n);
            array<System::Int32>^ arr = gcnew array<System::Int32>(res.size());
            if (arr->Length > 0) {
                pin_ptr<int32_t> arr_ptr = &arr[0];
                memcpy(arr_ptr, res.data(), arr->Length * sizeof(int32_t));
            }
            return arr;
        }

        static void boxBlur_4(array<System::Byte>^ source, array<System::Byte>^ dest, System::Int32 w, System::Int32 h, System::Int32 r) {
            pin_ptr<uint8_t> src = &source[0];
            pin_ptr<uint8_t> dst = &dest[0];
            LucasAlias::NINA::NinaPP::Image::ImageAnalysis::boxBlur_4(src, source->Length, dst, w, h, r);
        }

        static void boxBlurH_4(array<System::Byte>^ source, array<System::Byte>^ dest, System::Int32 w, System::Int32 h, System::Int32 r) {
            pin_ptr<uint8_t> src = &source[0];
            pin_ptr<uint8_t> dst = &dest[0];
            LucasAlias::NINA::NinaPP::Image::ImageAnalysis::boxBlurH_4(src, dst, w, h, r);
        }

        static void boxBlurT_4(array<System::Byte>^ source, array<System::Byte>^ dest, System::Int32 w, System::Int32 h, System::Int32 r) {
            pin_ptr<uint8_t> src = &source[0];
            pin_ptr<uint8_t> dst = &dest[0];
            LucasAlias::NINA::NinaPP::Image::ImageAnalysis::boxBlurT_4(src, dst, w, h, r);
        }
	};

}