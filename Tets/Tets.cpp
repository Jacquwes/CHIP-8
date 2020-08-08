#include <chrono>
#include <fstream>
#include <iostream>

#include "CHIP-8/chip8.h"

int main()
{
    Chip8::Chip8 chip8;

	std::array<uint8_t, 0x1000 - 0x200> raw_program;
	std::array<uint8_t, 0x200> fontset;

	std::ifstream input("../Assembler/test.cc8", std::ios::binary | std::ios::in);
	uint16_t i = 0;
	if (input)
		for (uint8_t temp; input.read(reinterpret_cast<char*>(&temp), sizeof(temp)); i++)
			raw_program[i] = temp;
	input.close();

	input.open("fontset.fc8", std::ios::binary | std::ios::in);
	i = 0;
	if (input)
		for (uint8_t temp; input.read(reinterpret_cast<char*>(&temp), sizeof(temp)); i++)
			fontset[i] = temp;
	input.close();

	chip8.LoadFontset(fontset);
    chip8.LoadProgram(raw_program);

	uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	chip8.RunProgram();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - start;

	return 0;
}
