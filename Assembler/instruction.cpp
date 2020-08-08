#include "instruction.h"

#include "constants.h"
#include "exception.h"
#include "program.h"

uint16_t Instruction::GetOpcode()
{
	uint16_t opcode = 0;
	if (operation_ == "clear")
		opcode = kClearScreen;
	else if (operation_ == "return")
		opcode = kReturn;
	else if (operation_ == "goto")
	{
		opcode = kGoto;
		bool subroutine_exists = false;
		for (const std::pair<std::string, Subroutine>& subroutine : program_->subroutines())
		{
			if (subroutine.first != operands_.front())
				for (int i = 0; i < subroutine.second.instructions.size(); i++)
					opcode += 2;
			else
			{
				subroutine_exists = true;
				break;
			}
		}
		if (!subroutine_exists)
			throw Exception(0, "Subroutine \"" + operands_.front() + "\" doesn't exist.", ExceptionType::kUnknownSubroutine);
	}
	else if (operation_ == "call")
	{
		opcode = kCallSubroutine;
		bool subroutine_exists = false;
		for (const std::pair<std::string, Subroutine>& subroutine : program_->subroutines())
		{
			if (subroutine.first != operands_.front())
				for (int i = 0; i < subroutine.second.instructions.size(); i++)
					opcode += 2;
			else
			{
				subroutine_exists = true;
				break;
			}
		}
		if (!subroutine_exists)
			throw Exception(0, "Subroutine \"" + operands_.front() + "\" doesn't exist.", ExceptionType::kUnknownSubroutine);
	}
	else if (operation_ == "skipeq")
	{
		opcode = kSkipNextIfVxEquals | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16);
	}

	return opcode;
}
