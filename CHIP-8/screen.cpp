#include "screen.h"

namespace Chip8
{
	Screen::Screen()
	{
		console_ = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL
		);
		SetConsoleActiveScreenBuffer(console_);

		Clear();
		updated_pixels_ = 0;

		UpdatePixels();
	}

	void Screen::Clear()
	{
		pixels_.fill(L' ');
		UpdatePixels();
	}

	void Screen::DrawSprite(const std::vector<std::bitset<8>>& sprite, uint8_t x_coordinate, uint8_t y_coordinate)
	{
		for (uint16_t y_index = 0; y_index < sprite.size(); y_index++)
		{
			for (int8_t x_index = 7; x_index >= 0; x_index--)
			{
				uint16_t& pixel = pixels_[x_coordinate + (y_coordinate * 64) + x_index + (y_index * 64)];
				if (sprite[y_index].test(x_index))
					pixel = L'█';
			}
		}
		UpdatePixels();
	}

	void Screen::UpdatePixels()
	{
		WriteConsoleOutputCharacter(
			console_,
			(LPCWSTR)&pixels_[0],
			Constants::kScreenSize,
			{ 0, 0 },
			(LPDWORD)&updated_pixels_
		);
	}
} // namespace Chip8