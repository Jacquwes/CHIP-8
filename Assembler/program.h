#pragma once

#include <string>
#include <map>
#include <vector>

#include "instruction.h"

class Instruction;

// idk why this exists
struct Subroutine
{
	std::vector<Instruction> instructions;
};

class Program
{
public:
	std::vector<uint8_t> Compile();
	void Parse(const std::string& raw_program);
	inline std::map<std::string, Subroutine> subroutines()
	{
		return subroutines_;
	}

private:
	inline void AddInstruction(const Instruction& instruction, const std::string& subroutine_name)
	{
		if (subroutines_.find(subroutine_name) == subroutines_.end())
			subroutines_[subroutine_name] = Subroutine();
		subroutines_[subroutine_name].instructions.push_back(instruction);
	}

	std::map<std::string, Subroutine> subroutines_;
};
