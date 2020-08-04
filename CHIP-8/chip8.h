#pragma once

#include <array>

class Chip8
{
public:
	Chip8();

private:
	// Components
	std::array<uint8_t, 0x1000> m_Memory;
	std::array<uint8_t, 0x10> m_Registers;

	uint16_t m_IndexRegister;
	uint16_t m_ProgramCounter; // 
};
