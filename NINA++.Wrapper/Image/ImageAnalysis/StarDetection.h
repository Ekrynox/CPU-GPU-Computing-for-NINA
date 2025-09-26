#pragma once

#include "Image/ImageAnalysis/StarDetection.hpp"



namespace LucasAlias::NINA::NinaPP::Image::ImageAnalysis {

	public ref class Patch_StarDetection {
	public:
		ref class Patch_Star {
		public:
			static void Calculate(System::Collections::Generic::List<StarDetection::PixelData^>^% pixelData, ::Accord::Point% _Position, System::Drawing::Rectangle% _Rectangle, double% _Average, double% _HFR, const double _Radius, const double _SurroundingMean) {
				auto cpixelData = std::vector<CPPStarDetection::PixelData>();
				for each (auto p in pixelData) {
					auto pd = CPPStarDetection::PixelData();
					pd.PosX = p->PosX;
					pd.PosY = p->PosY;
					pd.Value = p->Value;
					cpixelData.push_back(pd);
				}

				auto pos = CppPoint();
				pos.X = _Position.X;
				pos.Y = _Position.Y;

				auto rec = CppRectangle();
				rec.X = _Rectangle.X;
				rec.Y = _Rectangle.Y;
				rec.Width = _Rectangle.Width;
				rec.Height = _Rectangle.Height;

				double avg = _Average;
				double hfr = _HFR;

				CPPStarDetection::CPPStar::Calculate(cpixelData, pos, rec, avg, hfr, _Radius, _SurroundingMean);

				_Average = avg;
				_HFR = hfr;

				_Position.X = pos.X;
				_Position.Y = pos.Y;
			}
		};

	};

}