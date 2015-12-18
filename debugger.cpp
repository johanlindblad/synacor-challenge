#include "debugger.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <string>

Debugger::Debugger(VM vm) : step_mode(true)
{
	this->vm = vm;
	this->step_mode = false;
}

void Debugger::run()
{
	while(true)
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

		free(input);

		if(parts.size() == 0)
		{
			parts.push_back("");
		}

		std::string command = parts[0];

		if(command == "help")
		{
			std::cout << "List of commands:" << std::endl;
			std::cout << "<> # Empty command equals run" << std::endl;
			std::cout << "memory <X> # Dump the value of X, where X is a memory address" << std::endl;
			std::cout << "pc # Dump the program counter" << std::endl;
			std::cout << "register <X> # Dump the value of register X" << std::endl;
			std::cout << "run # Run the program" << std::endl;
			std::cout << "step <X> # Step X (default: 1) instructions instruction, except continuing on output" << std::endl;
		}
		else if(command == "memory")
		{
			std::cout << vm.get_memory(stoi(parts[1])) << std::endl;
		}
		else if(command == "pc")
		{
			std::cout << vm.get_program_counter() << std::endl;
		}
		else if(command == "register")
		{
			std::cout << vm.get_register(stoi(parts[1])) << std::endl;
		}
		else if(command == "run" || command == "")
		{
			vm.step(! step_mode);

			if(vm.is_halted())
			{
				std::cout << "VM is halted" << std::endl;
			}
		}
		else if(command == "step")
		{
			size_t num_steps = 1;

			if(parts.size() > 1)
			{
				num_steps = stoi(parts[1]);
			}

			for(int i=0; i < num_steps; i++)
			{
				vm.step(true);
			}
		}
		else	
		{
			std::cout << "Unrecognized command \"" << command << "\"" << std::endl;
		}
	}
}

bool Debugger::should_step()
{
	return step_mode;
}