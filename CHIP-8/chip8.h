#pragma once

#include <array>
#include <bitset>

#include "constants.h"
#include "screen.h"

namespace Chip8
{
	class Chip8
	{
	public:
		Chip8();

		void LoadFontset(const std::array<uint8_t, Constants::kFontsetMaxSize>& fontset);
		void LoadProgram(const std::array<uint8_t, Constants::kProgramMaxSize>& program);

		void RunProgram();

	private:
		std::array<uint8_t, Constants::kMemorySize> memory_;
		std::array<uint8_t, Constants::kRegistersSize> registers_;

		std::array<uint16_t, Constants::kStackSize> stack_;
		uint8_t stack_pointer_;

		std::array<uint8_t, 16> keymap_;
		std::bitset<0x10> keys_;

		Screen screen_;

		uint16_t index_register_;
		uint16_t program_counter_;

		uint8_t delay_timer_;
		uint8_t sound_timer_;

		uint64_t last_tick_;

		bool ended_;

		void Cycle();
	};
} // namespace Chip8