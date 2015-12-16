#include "debugger.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <string>

Debugger::Debugger(VM vm) : step_mode(true)
{
	this->vm = vm;
}

void Debugger::run()
{
	while(! vm.is_halted())
	{
		vm.step(true);

		if(should_step())
		{
			handle_command();
		}
	}
}

void Debugger::handle_command()
{
	char* input = readline("> ");

	if(input && *input)
	{
		add_history (input);
	}

	std::vector<std::string> parts;
	char* part = strtok(input, " ");

	while(part != NULL)
	{
		parts.push_back(std::string(part));
		part = strtok(NULL, " ");
	}

	if(parts.size() == 0)
	{
		free(input);
		return;	
	}

	std::string command = parts[0];

	if(command == "help")
	{
		std::cout << "List of commands:" << std::endl;
	}
	else
	{
		std::cout << "Unrecognized command \"" << command << "\"" << std::endl;
	}

	free(input);
}

bool Debugger::should_step()
{
	return step_mode;
}