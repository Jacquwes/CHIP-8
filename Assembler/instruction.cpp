#include "instruction.h"

#include "constants.h"
#include "exception.h"
#include "program.h"

uint16_t Instruction::GetOpcode()
{
	uint16_t opcode = 0;



	// display
	if (operation_ == "clear")
		opcode = kClearScreen;
	else if (operation_ == "draw")
		opcode = kDrawSprite | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4) | std::stoi(operands_[2], nullptr, 16);
	else if (operation_ == "getsprite")
		opcode = kSetIToChar | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "getbcd")
		opcode = kSetIToBCD | (std::stoi(&operands_[0][1], nullptr, 16) << 8);




	// flow
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
	else if (operation_ == "jump")
		opcode = kJumpNNNPlusV0 | std::stoi(operands_[0], nullptr, 16);



	// conditions
	else if (operation_ == "skipeq")
		opcode = operands_[1][0] != 'v'
			? kSkipNextIfVxEquals | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16)		
			: kSkipNextIfVxEqualsVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "skipne")
		opcode = operands_[1][0] != 'v'
			? kSkipNextIfVxDifferent | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16)
			: kSkipNextIfVxDifferentVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "pressed")
		opcode = kSkipNextIfKeyPressed | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "npressed")
		opcode = kSkipNextIfKeyNotPressed | (std::stoi(&operands_[0][1], nullptr, 16) << 8);



	// assignments
	else if (operation_ == "set")
		opcode = operands_[1][0] != 'v'
			? kSetVxToNN | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16)
			: kSetVxToVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "seti")
		opcode = kSetI | std::stoi(operands_[0], nullptr, 16);
	else if (operation_ == "getdelay")
		opcode = kSetToDelayTimer | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "getkey")
		opcode = kReadKey | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "setdelay")
		opcode = kSetDelayTimer | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "setsound")
		opcode = kSetSoundTimer | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "dump")
		opcode = kStoreRegistersAtI | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "load")
		opcode = kFillRegistersFromI | (std::stoi(&operands_[0][1], nullptr, 16) << 8);




	// maths
	else if (operation_ == "add")
		opcode = operands_[1][0] != 'v'
			? kAddNNToVx | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16)
			: kVxPlusVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "or")
		opcode = kVxOrVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "and")
		opcode = kVxAndVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "xor")
		opcode = kVxXorVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "sub")
		opcode = kVxMinusVy | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "rsub")
		opcode = kVyMinuxVx | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | (std::stoi(&operands_[1][1], nullptr, 16) << 4);
	else if (operation_ == "shiftr")
		opcode = kVxRightShift | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "shiftl")
		opcode = kVxLeftShift | (std::stoi(&operands_[0][1], nullptr, 16) << 8);
	else if (operation_ == "rand")
		opcode = kRandAndNN | (std::stoi(&operands_[0][1], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16);
	else if (operation_ == "addi")
		opcode = kAddVxToI | (std::stoi(&operands_[0][1], nullptr, 16) << 8);



	// data
	else if (operation_ == "data")
		opcode = (std::stoi(operands_[0], nullptr, 16) << 8) | std::stoi(operands_[1], nullptr, 16);




	return opcode;
}
