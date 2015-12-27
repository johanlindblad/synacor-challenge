#include "debugger.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <string>

Debugger::Debugger(VM vm) : step_mode(true), breadcrumbs_active(false), instream(*(new std::stringstream()))
{
	vm.set_input_stream(&this->instream);
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
			std::cout << "breadcrumbs (start|stop) # Starts or stops breadcrumb mode" << std::endl;
			std::cout << "break <X> # Set breakpoint at address X" << std::endl;
			std::cout << "exit" << std::endl;
			std::cout << "instruction <X> # Dump the instruction at X or pc" << std::endl;
			std::cout << "input <X...> # Write input into the input stream" << std::endl;
			std::cout << "inputf <filename> # Use file $filename as input" << std::endl;
			std::cout << "memory <X> # Dump the value of X, where X is a memory address" << std::endl;
			std::cout << "pc # Dump the program counter" << std::endl;
			std::cout << "register <X> <Y> # Dump the value of register X, or set it to Y" << std::endl;
			std::cout << "run # Run the program" << std::endl;
			std::cout << "stack # Print the stack " << std::endl;
			std::cout << "step <X> # Step X (default: 1) instructions instruction, except continuing on output" << std::endl;
			std::cout << "unbreak <X> # Unset breakpoint at address X" << std::endl;
		}
		else if(command == "breadcrumbs" && parts[1] == "start")
		{
			breadcrumbs_active = true;
		}
		else if(command == "breadcrumbs" && parts[1] == "stop")
		{
			breadcrumbs_active = false;

			std::cout << "History: ";

			for(int pc : breadcrumbs)
			{
				std::cout << pc << " ";
			}
			std::cout << std::endl;

			breadcrumbs.clear();
		}
		else if(command == "break")
		{
			breakpoints.insert(stoi(parts[1]));
		}
		else if(command == "exit")
		{
			break;
		}
		else if(command == "input")
		{
			parts.erase(parts.begin());
			instream = std::stringstream();

			for(int i=0; i < parts.size(); i++)
			{
				instream << parts[i];
				if(i != parts.size() - 1)
				{
					instream << " ";
				}
			}

		}
		else if(command == "inputf")
		{
			std::ifstream file(parts[1]);

			if(file)
			{
				instream = std::stringstream();
				instream << file.rdbuf();
				file.close();
			}
		}
		else if(command == "instruction" || command == "i")
		{
			if(parts.size() == 1)
			{
				std::cout << vm.describe_instruction() << std::endl;
			}
			else
			{
				std::cout << vm.describe_instruction(stoi(parts[1])) << std::endl;
			}
		}
		else if(command == "memory")
		{
			std::cout << vm.get_memory(stoi(parts[1])) << std::endl;
		}
		else if(command == "pc")
		{
			if(parts.size() < 2)
			{
				std::cout << vm.get_program_counter() << std::endl;
			}
			else
			{
				vm.set_program_counter(stoi(parts[1]));
			}
		}
		else if(command == "register")
		{
			if(parts.size() > 2)
			{
				vm.set_register(stoi(parts[1]), stoi(parts[2]));
			}
			else
			{
				std::cout << vm.get_register(stoi(parts[1])) << std::endl;
			}
		}
		else if(command == "run" || command == "")
		{
			bool breakpoint = false;

			while(true)
			{
				bool has_input = instream.peek() != EOF;
				breadcrumbs.push_back(vm.get_program_counter());
				vm.step(false);

				if(vm.is_halted() || (vm.requests_input() && !has_input))
				{
					break;
				}
				else if(breakpoints.count(vm.get_program_counter()) > 0)
				{
					breakpoint = true;
					std::cout << "Breaking at " << vm.get_program_counter() << std::endl;
					break;
				}
			}

			if(vm.requests_input() && !breakpoint)
			{
				std::cout << "VM requesting input: ";
				std::string buffer;
				instream = std::stringstream();
				std::getline(std::cin, buffer);
				instream << buffer << std::endl;
			}
			else if(vm.is_halted())
			{
				std::cout << "VM is halted" << std::endl;
			}
		}
		else if(command == "stack")
		{
			std::vector<uint16_t> values = vm.get_stack();

			for(uint16_t value : values)
			{
				std::cout << value << " ";
			}
			std::cout << std::endl;
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
				vm.step(false);
			}
		}
		else if(command == "unbreak")
		{
			breakpoints.erase(stoi(parts[1]));
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