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

#include "../../ninacl_internal.hpp"

#include <cmath>
#include <algorithm>
#include <execution>



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

    void debayerPattern(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, uint16_t* Larr, const bool __MT) {
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Rarr[widthM1] = tmpdst[RGB::B];
                Garr[widthM1] = tmpdst[RGB::G];
                Barr[widthM1] = tmpdst[RGB::R];
                Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);



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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);



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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Rarr[widthM1] = tmpdst[RGB::B];
                Garr[widthM1] = tmpdst[RGB::G];
                Barr[widthM1] = tmpdst[RGB::R];
                Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
            }
            else {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbValues[BayerPattern[0]] += *src;

                dst[RGB::R] = (uint16_t)rgbValues[RGB::R];
                dst[RGB::G] = (uint16_t)rgbValues[RGB::G];
                dst[RGB::B] = (uint16_t)rgbValues[RGB::B];
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
            }
        }



        // Main part of picture
        if (__MT) {
            if (heightM1 >= 2) {
                std::vector<int32_t> indices(heightM1 - 1);
                std::iota(indices.begin(), indices.end(), 1);
                std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [src, width, widthM1, srcOffset, srcStride, &dst, dstOffset, &BayerPattern, BPCols, &Rarr, &Garr, &Barr, &Larr](int32_t y) {
                    uint32_t rgbValues[3];
                    uint32_t rgbCounters[3];

                    auto counter = y * width + 1;
                    auto tmpsrc = src + (srcOffset + width) * y + 1;
                    auto tmpdst = dst + (dstOffset + 3 * width) * y + 3;
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
                        Rarr[counter] = tmpdst[RGB::B];
                        Garr[counter] = tmpdst[RGB::G];
                        Barr[counter] = tmpdst[RGB::R];
                        Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                        counter++;
                    }
                    });
            }
        }
        else {
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
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                    counter++;
                }
            }
        }
    }

    void debayerPatternRGB(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, const bool __MT) {
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];


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
                Rarr[widthM1] = tmpdst[RGB::B];
                Garr[widthM1] = tmpdst[RGB::G];
                Barr[widthM1] = tmpdst[RGB::R];

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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];


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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];


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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];


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
                Rarr[widthM1] = tmpdst[RGB::B];
                Garr[widthM1] = tmpdst[RGB::G];
                Barr[widthM1] = tmpdst[RGB::R];
            }
            else {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbValues[BayerPattern[0]] += *src;

                dst[RGB::R] = (uint16_t)rgbValues[RGB::R];
                dst[RGB::G] = (uint16_t)rgbValues[RGB::G];
                dst[RGB::B] = (uint16_t)rgbValues[RGB::B];
                Rarr[0] = dst[RGB::B];
                Garr[0] = dst[RGB::G];
                Barr[0] = dst[RGB::R];
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

                bayerIndex = BayerPattern[((x - 1) & 1)];
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
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
                Rarr[counter] = tmpdst[RGB::B];
                Garr[counter] = tmpdst[RGB::G];
                Barr[counter] = tmpdst[RGB::R];
            }
        }



        // Main part of picture
        if (__MT) {
            if (heightM1 >= 2) {
                std::vector<int32_t> indices(heightM1 - 1);
                std::iota(indices.begin(), indices.end(), 1);
                std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&src, width, widthM1, srcOffset, srcStride, &dst, dstOffset, &BayerPattern, BPCols, &Rarr, &Garr, &Barr](int32_t y) {
                    uint32_t rgbValues[3];
                    uint32_t rgbCounters[3];

                    auto counter = y * width + 1;
                    auto tmpsrc = src + (srcOffset + width) * y + 1;
                    auto tmpdst = dst + (dstOffset + 3 * width) * y + 3;
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
                        Rarr[counter] = tmpdst[RGB::B];
                        Garr[counter] = tmpdst[RGB::G];
                        Barr[counter] = tmpdst[RGB::R];
                        counter++;
                    }
                    });
            }
        }
        else {
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
                    Rarr[counter] = tmpdst[RGB::B];
                    Garr[counter] = tmpdst[RGB::G];
                    Barr[counter] = tmpdst[RGB::R];
                    counter++;
                }
            }
        }

    }

    void debayerPatternL(const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, const int32_t BPCols, uint16_t* Larr, const bool __MT) {
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
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);



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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);



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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);


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
                Larr[widthM1] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
            }
            else {
                rgbValues[0] = rgbValues[1] = rgbValues[2] = 0;
                rgbValues[BayerPattern[0]] += *src;

                dst[RGB::R] = (uint16_t)rgbValues[RGB::R];
                dst[RGB::G] = (uint16_t)rgbValues[RGB::G];
                dst[RGB::B] = (uint16_t)rgbValues[RGB::B];
                Larr[0] = (uint16_t)std::floor(((uint32_t)dst[RGB::R] + (uint32_t)dst[RGB::G] + (uint32_t)dst[RGB::B]) / 3.0);
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

                bayerIndex = BayerPattern[((x - 1) & 1)];
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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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

                bayerIndex = BayerPattern[((y + 1) & 1) * BPCols + 1];
                rgbValues[bayerIndex] += tmpsrc[srcStride + 1];
                rgbCounters[bayerIndex]++;

                tmpdst[RGB::R] = (uint16_t)(rgbValues[RGB::R] / rgbCounters[RGB::R]);
                tmpdst[RGB::G] = (uint16_t)(rgbValues[RGB::G] / rgbCounters[RGB::G]);
                tmpdst[RGB::B] = (uint16_t)(rgbValues[RGB::B] / rgbCounters[RGB::B]);
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
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
                Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
            }
        }



        // Main part of picture
        if (__MT) {
            if (heightM1 >= 2) {
                std::vector<int32_t> indices(heightM1 - 1);
                std::iota(indices.begin(), indices.end(), 1);
                std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&src, width, widthM1, srcOffset, srcStride, &dst, dstOffset, &BayerPattern, BPCols, &Larr](int32_t y) {
                    uint32_t rgbValues[3];
                    uint32_t rgbCounters[3];

                    auto counter = y * width + 1;
                    auto tmpsrc = src + (srcOffset + width) * y + 1;
                    auto tmpdst = dst + (dstOffset + 3 * width) * y + 3;
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
                        Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                        counter++;
                    }
                    });
            }
        }
        else {
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
                    Larr[counter] = (uint16_t)std::floor(((uint32_t)tmpdst[RGB::R] + (uint32_t)tmpdst[RGB::G] + (uint32_t)tmpdst[RGB::B]) / 3.0);
                    counter++;
                }
            }
        }

    }


    void debayerPatternOpenCL(OpenCLManager& opCLM, size_t context, const int32_t width, const int32_t height, uint16_t* src, uint16_t* dst, const int32_t srcStride, int32_t srcOffset, int32_t dstOffset, int32_t* const BayerPattern, uint16_t* Rarr, uint16_t* Garr, uint16_t* Barr, uint16_t* Larr) {
        auto exctx = opCLM.GetImpl().getExecutionContext(context);

        auto srcBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, height * srcStride * sizeof(uint16_t));
        auto dstBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, height * (3 * width + dstOffset) * sizeof(uint16_t));
        auto bayerBuffer = cl::Buffer(exctx.context, CL_MEM_READ_ONLY, 2 * 2 * sizeof(int32_t));

        auto RarrBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, height * width * sizeof(uint16_t));
        auto GarrBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, height * width * sizeof(uint16_t));
        auto BarrBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, height * width * sizeof(uint16_t));
        auto LarrBuffer = cl::Buffer(exctx.context, CL_MEM_WRITE_ONLY, height * width * sizeof(uint16_t));

        exctx.commandQ.enqueueWriteBuffer(srcBuffer, CL_FALSE, 0, height * srcStride * sizeof(uint16_t), src);
        exctx.commandQ.enqueueWriteBuffer(bayerBuffer, CL_FALSE, 0, 2 * 2 * sizeof(int32_t), BayerPattern);

        int localX = 16, localY = 16;
        size_t globalX = ((width + localX - 1) / localX) * localX;
        size_t globalY = ((height + localY - 1) / localY) * localY;
        cl::NDRange global(globalY, globalX);
        cl::NDRange local(localY, localX);

        auto kernel = cl::Kernel(exctx.programs[L"BayerFilter16bpp.cl"], "debayerPattern");
        int arg = 0;
        kernel.setArg(arg++, width);
        kernel.setArg(arg++, height);
        kernel.setArg(arg++, localX);
        kernel.setArg(arg++, localY);
        kernel.setArg(arg++, srcBuffer);
        kernel.setArg(arg++, dstBuffer);
        kernel.setArg(arg++, (localX + 2) * (localY + 2) * sizeof(uint16_t), nullptr);
        kernel.setArg(arg++, srcStride);
        kernel.setArg(arg++, srcOffset);
        kernel.setArg(arg++, dstOffset);
        kernel.setArg(arg++, bayerBuffer);
        kernel.setArg(arg++, RarrBuffer);
        kernel.setArg(arg++, GarrBuffer);
        kernel.setArg(arg++, BarrBuffer);
        kernel.setArg(arg++, LarrBuffer);

        exctx.commandQ.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

        std::vector<cl::Event> readEvents;
        cl::Event e1, e2, e3, e4, e5;
        exctx.commandQ.enqueueReadBuffer(dstBuffer, CL_FALSE, 0, height * (3 * width + dstOffset) * sizeof(uint16_t), dst, nullptr, &e1);
        exctx.commandQ.enqueueReadBuffer(RarrBuffer, CL_FALSE, 0, height * width * sizeof(uint16_t), Rarr, nullptr, &e2);
        exctx.commandQ.enqueueReadBuffer(GarrBuffer, CL_FALSE, 0, height * width * sizeof(uint16_t), Garr, nullptr, &e3);
        exctx.commandQ.enqueueReadBuffer(BarrBuffer, CL_FALSE, 0, height * width * sizeof(uint16_t), Barr, nullptr, &e4);
        exctx.commandQ.enqueueReadBuffer(LarrBuffer, CL_FALSE, 0, height * width * sizeof(uint16_t), Larr, nullptr, &e5);
        readEvents = { e1, e2, e3, e4, e5 };
        cl::Event::waitForEvents(readEvents);

    }
}