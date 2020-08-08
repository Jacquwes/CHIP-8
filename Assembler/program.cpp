#include "program.h"

#include "instruction.h"

std::vector<uint8_t> Program::Compile()
{
	std::vector<uint8_t> compiled;
	for (std::pair<std::string, Subroutine> subroutine : subroutines_)
		for (Instruction& instruction : subroutine.second.instructions)
		{
			uint16_t opcode = instruction.GetOpcode();
			compiled.insert(compiled.end(), { static_cast<uint8_t>((opcode & 0xff00) >> 8), static_cast<uint8_t>(opcode & 0x00ff) });
		}
	return compiled;
}

void Program::Parse(const std::string& raw_program)
{
	std::vector<std::string> lines{ "" };

	bool is_comment = false;
	for (const char& character : raw_program)
		if (character == '\r' || character == '\n')
		{
			if (!lines.back().empty())
				lines.push_back("");
			is_comment = false;
		}
		else if (character == ';')
			is_comment = true;
		else if (!is_comment && character != '\t')
			lines.back() += character;

	while (lines.back().empty())
		lines.pop_back();

	std::string current_subroutine = "main_";

	for (const std::string& line : lines)
	{
		// subroutine declaration
		if (line.back() == ':')
		{
			// Todo: throw exception if subroutine name contains space
			// Todo: or if name == "main_"
			current_subroutine = line.substr(0, line.size() - 1);
		}
		else
		{
			// operation
			std::string operation;
			int i = 0;
			while (line[i] != ' ' && i < line.size())
				operation += line[i++];

			// operands
			std::vector<std::string> operands{ "" };
			for (; i < line.size(); i++)
			{
				const char& character = line[i];
				if (character == ' ' || character == ',')
				{
					if (!operands.back().empty())
						operands.push_back("");
				}
				else
					operands.back() += character;
			}
			while (!operands.empty() && operands.back().empty())
				operands.pop_back();
			while (!operands.empty() && operands.front().empty())
				operands.erase(operands.begin());

			Instruction instruction(this, operation, operands);

			// add to program
			AddInstruction(instruction, current_subroutine);
		}
	}
}