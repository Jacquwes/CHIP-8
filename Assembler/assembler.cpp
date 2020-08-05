#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

struct Subroutine
{

};

struct Program
{
	std::vector<Subroutine> subroutines;
};

int main()
{
	std::string file_name, program;
	std::vector<std::string> lines { "" }, compiled;
	

	std::cout << "File to compile: ";
	std::cin >> file_name;
	std::ifstream input(file_name, std::ios::binary);
	input.read(&program[0], sizeof(uint8_t));

	bool is_comment;
	for (char& character : program)
		if (character == '\n')
		{
			lines.push_back("");
			is_comment = false;
		}
		else if (character == ';')
			is_comment = true;
		else
			lines.back().append(&character);

	while (lines.back().empty())
		lines.pop_back();

	for (std::string& line : lines)
	{
		std::vector<std::string> words { "" };
		for (char& character : line)
			if (character == ' ')
				words.push_back(" ");
			else
				words.back().append(&character);
		while (words.back().empty())
			words.pop_back();


	}
}
