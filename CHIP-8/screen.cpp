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

		pixels_			.fill(0);
		updated_pixels_ = 0;
	}

	void Screen::Clear()
	{
		pixels_.fill(0);
		UpdatePixels();
	}

	void Screen::UpdatePixels()
	{
		WriteConsoleOutputCharacter(
			console_,
			(LPCWSTR)&pixels_.begin(),
			Constants::kScreenSize,
			{ 0, 0 },
			(LPDWORD)&updated_pixels_
		);
	}
} // namespace Chip8