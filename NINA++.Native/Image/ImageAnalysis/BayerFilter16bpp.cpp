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

#include "BayerFilter16bpp.hpp"

#include <cmath>



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

    void copyImage(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols) {
        // for each line
        for (int32_t y = 0; y < height; y++) {
            // for each pixel
            for (int32_t x = 0; x < width; x++, src++, dst += 3) {
                dst[RGB::R] = 0;
                dst[RGB::G] = 0;
                dst[RGB::B] = 0;
                dst[BayerPattern[(y & 1) * BPCols + (x & 1)]] = *src;
            }
            src += srcOffset;
            dst += dstOffset;
        }
    }

    void debayerPattern(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, const int32_t dstStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, uint16_t* Larr) {
        int32_t widthM1 = width - 1;
        int32_t heightM1 = height - 1;

        uint32_t rgbValues[3];
        uint32_t rgbCounters[3];

        int32_t counter;
        uint16_t* tmpsrc;
        uint16_t* tmpdst;


        //Corners
        if (height > 1) {
            if (width > 1) {
                // Corner
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[0];
                rgbValues[bayerIndex] += *src;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[1];
                rgbValues[bayerIndex] += src[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols];
                rgbValues[bayerIndex] += src[srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + 1];
                rgbValues[bayerIndex] += src[srcStride + 1];
                rgbCounters[bayerIndex]++;

                dst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                dst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                dst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[0] = dst[RGB::B];
                    Garr[0] = dst[RGB::G];
                    Barr[0] = dst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
                }


                // Corner
                tmpsrc = src + widthM1;
                tmpdst = dst + 3 * widthM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                bayerIndex = BayerPattern[(widthM1 & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + (widthM1 & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + ((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[widthM1] = tmpdst[RGB::B];
                    Garr[widthM1] = tmpdst[RGB::G];
                    Barr[widthM1] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }


                // Corner
                counter = heightM1 * width + widthM1;
                tmpsrc = src + (srcOffset + width) * heightM1 + widthM1;
                tmpdst = dst + (dstOffset + 3 * width) * heightM1 + 3 * widthM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + (widthM1 & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + ((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + (widthM1 & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + ((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }


                // Corner
                counter = heightM1 * width;
                tmpsrc = src + (srcOffset + width) * heightM1;
                tmpdst = dst + (dstOffset + 3 * width) * heightM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }
            else {
                // Corner
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[0];
                rgbValues[bayerIndex] += *src;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols];
                rgbValues[bayerIndex] += src[srcStride];
                rgbCounters[bayerIndex]++;

                dst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                dst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                dst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[0] = dst[RGB::B];
                    Garr[0] = dst[RGB::G];
                    Barr[0] = dst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
                }


                // Corner
                counter = heightM1 * width;
                tmpsrc = src + (srcOffset + width) * heightM1;
                tmpdst = dst + (dstOffset + 3 * width) * heightM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }
        }
        else {
            if (width > 1) {
                // Corner
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[0];
                rgbValues[bayerIndex] += *src;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[1];
                rgbValues[bayerIndex] += src[1];
                rgbCounters[bayerIndex]++;

                dst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                dst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                dst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[0] = dst[RGB::B];
                    Garr[0] = dst[RGB::G];
                    Barr[0] = dst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
                }


                // Corner
                tmpsrc = src + widthM1;
                tmpdst = dst + 3 * widthM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                bayerIndex = BayerPattern[(widthM1 & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[widthM1] = tmpdst[RGB::B];
                    Garr[widthM1] = tmpdst[RGB::G];
                    Barr[widthM1] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }
            else {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbValues[BayerPattern[0]] += *src;

                dst[RGB::R] = (uint16_t)rgbValues[RGB::R];
                dst[RGB::G] = (uint16_t)rgbValues[RGB::G];
                dst[RGB::B] = (uint16_t)rgbValues[RGB::B];
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[0] = dst[RGB::B];
                    Garr[0] = dst[RGB::G];
                    Barr[0] = dst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
                }
            }
        }



        // First line & Last Line
        if (height > 1) {
            //First Line
            counter = width + 1;
            tmpsrc = src + 1;
            tmpdst = dst + 3;
            // for each pixel
            for (int32_t x = 1; x < widthM1; x++, tmpsrc++, tmpdst += 3) {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(x & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[ ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + (x & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
                counter++;
            }

            //Last Line
            counter = heightM1 * width + 1;
            tmpsrc = src + (srcOffset + width) * heightM1 + 1;
            tmpdst = dst + (dstOffset + 3 * width) * heightM1 + 3;
            // for each pixel
            for (int32_t x = 1; x < widthM1; x++, tmpsrc++, tmpdst += 3) {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + (x & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(heightM1 & 1) * BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + (x & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((heightM1 - 1) & 1) * BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
                counter++;
            }
        }
        else {
            counter = width + 1;
            tmpsrc = src + 1;
            tmpdst = dst + 3;
            // for each pixel
            for (int32_t x = 1; x < widthM1; x++, tmpsrc++, tmpdst += 3) {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(x & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
                counter++;
            }
        }



        // First col & Last col
        if (width > 1) {
            // First col
            for (int32_t y = 1; y < heightM1; y++) {
                counter = y * width;
                tmpsrc = src + (srcOffset + width) * y;
                tmpdst = dst + (dstOffset + 3 * width) * y;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(y & 1) * BPCols];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;
              
                bayerIndex = BayerPattern[(y & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;
                
                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;
                
                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols +1];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }

            // Last col
            for (int32_t y = 1; y < heightM1; y++) {
                counter = y * width + widthM1;
                tmpsrc = src + (srcOffset + width) * y + widthM1;
                tmpdst = dst + (dstOffset + 3 * width) * y + 3 * widthM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(y & 1) * BPCols + (widthM1 & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(y & 1) * BPCols + ((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(y & 1) * BPCols + ((widthM1 + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + (widthM1 & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((widthM1 - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((widthM1 + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }
        }
        else {
            for (int32_t y = 1; y < heightM1; y++) {
                counter = y * width + widthM1;
                tmpsrc = src + (srcOffset + width) * y + widthM1;
                tmpdst = dst + (dstOffset + 3 * width) * y + 3 * widthM1;

                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(y & 1) * BPCols];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols];
                rgbValues[bayerIndex] += tmpsrc[srcStride];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
            }
        }



        // Main part of picture
        // for each line
        for (int32_t y = 1; y < heightM1; y++) {
            counter = y * width + 1;
            tmpsrc = src + (srcOffset + width) * y + 1;
            tmpdst = dst + (dstOffset + 3 * width) * y + 3;
            // for each pixel
            for (int32_t x = 1; x < widthM1; x++, tmpsrc++, tmpdst += 3) {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbCounters[0] = rgbCounters[1] = rgbCounters[2] = 0;

                int32_t bayerIndex = BayerPattern[(y & 1) * BPCols + (x & 1)];
                rgbValues[bayerIndex] += *tmpsrc;
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(y & 1) * BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[(y & 1) * BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + (x & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride - 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y - 1) & 1) * BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[-srcStride + 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + (x & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + ((x - 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride - 1];
                rgbCounters[bayerIndex]++;

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + ((x + 1) & 1)];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                if ((Rarr != nullptr) && (Garr != nullptr) && (Barr != nullptr)) {
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                }
                if (Larr != nullptr) {
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                }
                counter++;
            }
        }

    }

}