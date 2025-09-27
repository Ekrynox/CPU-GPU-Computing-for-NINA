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

#include "StarDetection.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis::CPPStarDetection {
    namespace CPPStar {

        static bool InsideCircle(double x, double y, double centerX, double centerY, double radius) {
            return (pow(x - centerX, 2) + pow(y - centerY, 2) <= pow(radius, 2));
        }

        void Calculate(const std::vector<PixelData>& pixelData, CppPoint& _Position, const CppRectangle& _Rectangle, double& _Average, double& _HFR, const double _Radius, const double _SurroundingMean) {
            double hfr = 0.0;
            if (pixelData.size() > 0) {
                double outerRadius = _Radius * 1.2;
                double sum = 0, sumDist = 0, allSum = 0, sumValX = 0, sumValY = 0;

                double centerX = _Position.X;
                double centerY = _Position.Y;

                for (const auto& data : pixelData) {
                    double value = round(data.Value - _SurroundingMean);
                    if (value < 0) {
                        value = 0;
                    }

                    allSum += value;
                    if (InsideCircle(data.PosX, data.PosY, _Position.X, _Position.Y, outerRadius)) {
                        sum += value;
                        sumDist += value * sqrt(pow(data.PosX - centerX, 2.0) + pow(data.PosY - centerY, 2.0));
                        sumValX += (data.PosX - _Rectangle.X) * value;
                        sumValY += (data.PosY - _Rectangle.Y) * value;
                    }
                }

                if (sum > 0) {
                    hfr = sumDist / sum;
                }
                else {
                    hfr = sqrt(2) * outerRadius;
                }
                _Average = allSum / pixelData.size();

                if (sum != 0) {
                    // Update the centroid
                    double centroidX = sumValX / sum;
                    double centroidY = sumValY / sum;
                    _Position = CppPoint();
                    _Position.X = (float)centroidX + _Rectangle.X;
                    _Position.Y = (float)centroidY + _Rectangle.Y;
                }
            }
            _HFR = hfr;
        }

    }
}