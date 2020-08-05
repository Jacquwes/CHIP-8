#pragma once

#include <Windows.h>

#include <array>

#include "constants.h"

namespace Chip8
{
	class Screen
	{
	public:
		Screen();

		void Clear();

	private:
		HANDLE console_;
		std::array<int16_t, Constants::kScreenSize> pixels_;
		int32_t updated_pixels_;

		void UpdatePixels();
	};
} // namespace Chip8