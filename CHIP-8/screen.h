#pragma once

#include <Windows.h>

#include <array>
#include <bitset>
#include <vector>

#include "constants.h"

namespace Chip8
{
	class Screen
	{
	public:
		Screen();

		void Clear();
		void DrawSprite(const std::vector<std::bitset<8>>& sprite, uint8_t x, uint8_t y);

	private:
		HANDLE console_;
		std::array<uint16_t, Constants::kScreenSize> pixels_;
		uint32_t updated_pixels_;

		void UpdatePixels();
	};
} // namespace Chip8