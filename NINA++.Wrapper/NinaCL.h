#pragma once
#include "ninacl.hpp"



namespace LucasAlias::NINA::NinaPP {
	public ref class CL {
	public:

		static System::UInt32 GetCLPlateformsNumber() {
			return getCLPlateformsNumber();
		}

	};
}