#include <fstream>
#include <iostream>

#include "program.h"

int main()
{
	std::string file_name, raw_program;	

	// ask file
	std::cout << "File to compile: ";
	std::cin >> file_name;
	std::ifstream input(file_name, std::ios::binary | std::ios::in);
	if (input)
		for (uint8_t temp; input.read(reinterpret_cast<char*>(&temp), sizeof(temp));)
			raw_program += temp;
	input.close();

	// output name
	std::cout << std::endl << "Output file name: ";
	std::cin >> file_name;

	// parse
	Program program;
	program.Parse(raw_program);

	// write compiled
	std::ofstream output(file_name, std::ios::binary);
	std::vector<uint8_t> compiled = program.Compile();
	output.write((char*)&compiled[0], compiled.size() * sizeof(uint8_t));
	output.close();
}
