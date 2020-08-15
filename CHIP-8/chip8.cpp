#include "chip8.h"

#include <chrono>
#include <functional>
#include <map>
#include <random>
#include <string>

#include <conio.h>
#include <Windows.h>

#include "exception.h"

namespace Chip8
{
	Chip8::Chip8()
	{
		memory_				.fill(0);
		registers_			.fill(0);

		stack_				.fill(0);
		stack_pointer_		= 0;

		// azerty			= { '1', '2', '3', '4', 'a', 'z', 'e', 'r', 'q', 's', 'd', 'f', 'w', 'x', 'c', 'v' };
		// qwerty			= { '1', '2', '3', '4', 'q', 'w', 'e', 'r', 'a', 's', 'd', 'f', 'z', 'x', 'c', 'v' };
		keymap_				= { '1', '2', '3', '4', 'A', 'Z', 'E', 'R', 'Q', 'S', 'D', 'F', 'W', 'X', 'C', 'V' };
		keys_				.reset();

		screen_				= Screen();

		index_register_		= 0;
		program_counter_	= Constants::kFontsetMaxSize;

		delay_timer_		= 0;
		sound_timer_		= 0;

		last_tick_			= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		ended_				= false;
	}

	void Chip8::LoadFontset(const std::array<uint8_t, Constants::kFontsetMaxSize>& fontset)
	{
		for (size_t i = 0; i < fontset.size(); i++)
			memory_[0x50 + i] = fontset[i];
	}

	void Chip8::LoadProgram(const std::array<uint8_t, Constants::kProgramMaxSize>& program)
	{
		for (size_t i = 0; i < program.size(); i++)
			memory_[i + 0x200] = program[i];
	}

	void Chip8::Cycle()
	{
		int16_t opcode	= memory_[program_counter_] << 8 
						| memory_[program_counter_ + 1];

		for (uint8_t i = 0; i < keymap_.size(); i++)
		{
			if (GetAsyncKeyState(keymap_[i]))
				keys_.set(i, true);
		}

		std::function<void()> UnknownOpcode = [&]()
		{ 
			throw Exception(program_counter_, ("Unknown opcode \"" + std::to_string(opcode) + "\" at address \"" + std::to_string(program_counter_)) + "\".", ExceptionType::kUnknownOpcode);
		};

		switch (opcode & 0xf000)
		{
		case 0x0000:
			switch (opcode)
			{
			case Constants::kClearScreen:
				screen_.Clear();
				break;
			case Constants::kReturn:
				if (!stack_pointer_)
					ended_ = true;
				else
				{
					program_counter_ = stack_[--stack_pointer_] + 2;
					stack_[stack_pointer_] = 0;
				}
				break;
			default:
				UnknownOpcode();
			}
			break;
		case Constants::kGoto:
			program_counter_ = (opcode & 0x0fff) + Constants::kFontsetMaxSize - 2;
			break;
		case Constants::kCallSubroutine:
			stack_[stack_pointer_++] = program_counter_;
			program_counter_ = (opcode & 0x0fff) + Constants::kFontsetMaxSize;
			break;
		case Constants::kSkipNextIfVxEquals:
			if ((registers_[(opcode & 0x0f00) >> 8]) == (opcode & 0x00ff))
				program_counter_ += 2;
			break;
		case Constants::kSkipNextIfVxDifferent:
			if ((registers_[(opcode & 0x0f00) >> 8]) != (opcode & 0x0ff))
				program_counter_ += 2;
			break;
		case Constants::kSkipNextIfVxEqualsVy:
			if ((registers_[(opcode & 0x0f00) >> 8]) == (registers_[(opcode & 0x00f0) >> 4]))
				program_counter_ += 2;
			break;
		case Constants::kSetVxtoNN:
			registers_[(opcode & 0x0f00) >> 8] = opcode & 0x00ff;
			break;
		case Constants::kAddNNToVx:
			registers_[(opcode & 0x0f00) >> 8] += opcode & 0x00ff;
			break;
		case 0x8000:
			switch (opcode & 0xf00f)
			{
			case Constants::kSetVxToVy:
				registers_[(opcode & 0x0f00) >> 8] = registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxOrVy:
				registers_[(opcode & 0x0f00) >> 8] |= registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxAndVy:
				registers_[(opcode & 0x0f00) >> 8] &= registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxXorVy:
				registers_[(opcode & 0x0f00) >> 8] ^= registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxPlusVy:
				if ((0x00ff - registers_[(opcode & 0x0f00) >> 8]) < registers_[(opcode & 0x00f0) >> 4])
					registers_[0xf] = 1;
				else
					registers_[0xf] = 0;
				registers_[(opcode & 0x0f00) >> 8] += registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxMinusVy:
				if (registers_[(opcode & 0x0f00) >> 8] < registers_[(opcode & 0x00f0) >> 4])
					registers_[0xf] = 0;
				else
					registers_[0xf] = 1;
				registers_[(opcode & 0x0f00) >> 8] -= registers_[(opcode & 0x00f0) >> 4];
				break;
			case Constants::kVxRightShift:
				registers_[0xf] = (registers_[(opcode & 0x0f00) >> 8] & 1);
				registers_[(opcode & 0x0f00) >> 8] >>= 1;
				break;
			case Constants::kVyMinuxVx:
				if (registers_[(opcode & 0x0f00) >> 8] < registers_[(opcode & 0x00f0) >> 4])
					registers_[0xf] = 0;
				else
					registers_[0xf] = 1;
				registers_[(opcode & 0x0f00) >> 8] = registers_[(opcode & 0x00f0) >> 4] - registers_[(opcode & 0x0f00) >> 8];
				break;
			case Constants::kVxLeftShift:
				registers_[0xf] = registers_[(opcode & 0x0f00) >> 8] >> 7;
				registers_[(opcode & 0x0f00) >> 8] <<= 1;
				break;
			default:
				UnknownOpcode();
			}
			break;
		case Constants::kSkipNextIfVxDifferentVy:
			if (registers_[(opcode & 0x0f00) >> 8] != registers_[(opcode & 0x00f0) >> 4])
				program_counter_ += 2;
			break;
		case Constants::kSetI:
			index_register_ = (opcode & 0x0fff) + Constants::kFontsetMaxSize;
			break;
		case Constants::kJumpNNNPlusV0:
			program_counter_ = registers_[0] + (opcode & 0x0fff);
			break;
		case Constants::kRandAndNN:
		{
			std::random_device device;
			std::default_random_engine engine(device());
			std::uniform_int_distribution<int> uniform_distribution(0, 0xff);
			registers_[(opcode & 0x0f00) >> 8] = (uniform_distribution(engine) & (opcode & 0x00ff));
			break;
		}
		case Constants::kDrawSprite:
		{
			uint8_t x		= registers_[(opcode & 0x0f00) >> 8];
			uint8_t y		= registers_[(opcode & 0x00f0) >> 4];
			uint8_t height	= opcode & 0x000f;
			std::vector<std::bitset<8>> sprite;
			for (int i = 0; i < height; i++)
				sprite.push_back(memory_[index_register_ + i]);
			screen_.DrawSprite(sprite, x, y);
			break;
		}
		case 0xe000:
			switch (opcode & 0xf0ff)
			{
			case Constants::kSkipNextIfKeyPressed:
				if (keys_.test(registers_[(opcode & 0x0f00) >> 8]))
					program_counter_ += 2;
				break;
			case Constants::kSkipNextIfKeyNotPressed:
				if (!keys_.test(registers_[(opcode & 0x0f00) >> 8]))
					program_counter_ += 2;
				break;
			default:
				UnknownOpcode();
			}
			break;
		case 0xf000:
			switch (opcode & 0xf0ff)
			{
			case Constants::kSetToDelayTimer:
				registers_[(opcode & 0x0f00) >> 8] = delay_timer_;
				break;
			case Constants::kReadKey:
				registers_[(opcode & 0x0f00) >> 8] = _getch();
				break;
			case Constants::kSetDelayTimer:
				delay_timer_ = registers_[(opcode & 0x0f00) >> 8];
				break;
			case Constants::kSetSoundTimer:
				sound_timer_ = registers_[(opcode & 0x0f00) >> 8];
				break;
			case Constants::kAddVxToI:
				index_register_ += registers_[(opcode & 0x0f00) >> 8];
				break;
			case Constants::kSetIToChar:
			{
				std::map<uint8_t, uint16_t> characters;
				for (uint8_t i = '0', n = 0; i <= 'F'; i++)
					if (i <= '9' || i >= 'A')
						characters[i] = n++;
				index_register_ = characters[registers_[(opcode & 0x0f00) >> 8]] * 5;
				break;
			}
			case Constants::kSetIToBCD:
				memory_[index_register_] = registers_[(opcode & 0x0f00) >> 8] / 100;
				memory_[index_register_ + 1] = (registers_[(opcode & 0x0f00) >> 8] / 10) % 10;
				memory_[index_register_ + 2] = registers_[(opcode & 0x0f00) >> 8] % 10;
				break;
			case Constants::kStoreRegistersAtI:
				for (uint8_t i = 0; i < ((opcode & 0x0f00) >> 8); i++)
					memory_[index_register_ + i] = registers_[i];
				break;
			case Constants::kFillRegistersFromI:
				for (uint8_t i = 0; i < ((opcode & 0x0f00) >> 8); i++)
					registers_[i] = memory_[index_register_ + i];
				break;
			default:
				UnknownOpcode();
			}
			break;
		default:
			UnknownOpcode();
		}

		if (delay_timer_)
			--delay_timer_;
		if (sound_timer_)
			--sound_timer_;

		program_counter_ += 2;
	}

	void Chip8::RunProgram()
	{
		while (!ended_)
		{
			int64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			if (current_time - last_tick_ >= std::llroundf((1.f / 60.f) * 1000))
			{
				last_tick_ = current_time;
				Cycle();
			}
		}
	}
} // namespace Chip8