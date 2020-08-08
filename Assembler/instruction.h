#pragma once

#include <string>
#include <vector>

class Program;

class Instruction
{
public:
	inline Instruction(
		Program* program,
		const std::string& operation,
		const std::vector<std::string>& operands
	)
		: program_(program)
		, operation_(operation)
		, operands_(operands)
	{}

	uint16_t GetOpcode();

private:
	std::vector<std::string> operands_;
	std::string operation_;
	Program* program_;
};
