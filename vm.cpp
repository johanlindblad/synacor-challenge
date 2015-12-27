#include "vm.hpp"

VM::VM() : program_counter(false), halted(false), instream(&std::cin)
{
	std::fill_n(memory, 32768, 0);
	std::fill_n(registers, 8, 0);
};

VM::VM(std::string binary_name) : program_counter(false), halted(false)
{
	std::ifstream f("challenge.bin", std::ios::in | std::ios::binary);
	f.read((char*) memory, 32768 * sizeof(uint16_t));

	std::fill_n(registers, 8, 0);
};

bool VM::step(bool continue_until_input)
{
	if(halted) return false;

	while(true)
	{
		uint16_t byte = memory[program_counter];

		// Potentially unsafe but will only be used after safe reads
		uint16_t a = memory[(program_counter + 1) % 32768];
		uint16_t b = memory[(program_counter + 2) % 32768];
		uint16_t c = memory[(program_counter + 3) % 32768];

		if(a == 32775 || b == 32775 || c == 32775)
		{
			//std::cout << std::to_string(get_program_counter()) << "!!!!!" << std::endl;
		}

		char character;

		switch(byte)
		{
			case 0:
				std::cout << "halt on " << program_counter << std::endl;
				halted = true;
				return true;
				break;
			case 1:
				a -= 32768;
				registers[a] = literal_value(b);

				program_counter += 3;
				break;
			case 2:
				stack.push(literal_value(a));

				program_counter += 2;
				break;
			case 3:
				if(stack.empty())
				{
					throw "Empty stack";
				}
				store_in(a, stack.top());
				stack.pop();

				program_counter += 2;
				break;
			case 4:
				store_in(a, (literal_value(b) == literal_value(c)) ? 1 : 0);

				program_counter += 4;
				break;
			case 5:
				store_in(a, (literal_value(b) > literal_value(c)) ? 1 : 0);

				program_counter += 4;
				break;
			case 6:
				program_counter = a;
				break;
			case 7:
				program_counter += 3;

				if(literal_value(a) > 0) program_counter = literal_value(b);

				break;
			case 8:
				program_counter += 3;

				if(literal_value(a) == 0) program_counter = literal_value(b);

				break;
			case 9:
				store_in(a, (literal_value(b) + literal_value(c)) % 32768);

				program_counter += 4;
				break;
			case 10:
				store_in(a, (literal_value(b) * literal_value(c)) % 32768);

				program_counter += 4;
				break;
			case 11:
				store_in(a, (literal_value(b) % literal_value(c)));

				program_counter += 4;
				break;
			case 12:
				store_in(a, literal_value(b) & literal_value(c));

				program_counter += 4;
				break;
			case 13:
				store_in(a, literal_value(b) | literal_value(c));

				program_counter += 4;
				break;
			case 14:
				store_in(a, ~literal_value(b) & ~(1 << 15));

				program_counter += 3;
				break;
			case 15:
			  b = memory[literal_value(b)];
				store_in(a, b);

				program_counter += 3;
				break;
			case 16:
				memory[literal_value(a)] = literal_value(b);

				program_counter += 3;
				break;
			case 17:
				program_counter += 2;

				stack.push(program_counter);
				program_counter = literal_value(a);

				break;
			case 18:
				program_counter = stack.top();
				stack.pop();
				break;
			case 19:
				std::cout << ((char) literal_value(a));

				program_counter += 2;
				break;
			case 20:
				if(instream->peek() == EOF)
				{
					std::cout << "halt on " << program_counter << std::endl;
					halted = true;
					return true;
				}

				character = instream->get();

				store_in(a, character);

				program_counter += 2;
				break;
			case 21:
				program_counter += 1;
				break;
			default:
				std::cerr << "Unhandled instruction " << byte << std::endl;
				halted = true;
				return false;
				break;
		}

		if(continue_until_input == false) break;
		if(memory[program_counter] == 20) break;
	}

	return true;
}

std::string VM::describe_literal(uint16_t literal)
{
	if(literal >= 32768)
	{
		uint16_t register_number = literal - 32768;
		return "r" + std::to_string(register_number) + " (current value " + std::to_string(registers[register_number]) + ")";
	}
	return std::to_string(literal);
}

std::string VM::describe_instruction()
{
	return describe_instruction(this->program_counter);
}

std::string VM::describe_instruction(uint16_t program_counter)
{
	uint16_t byte = memory[program_counter];

	// Potentially unsafe but will only be used after safe reads
	uint16_t a = memory[(program_counter + 1) % 32768];
	uint16_t b = memory[(program_counter + 2) % 32768];
	uint16_t c = memory[(program_counter + 3) % 32768];

	switch(byte)
	{
		case 0:
			return "halt";
		case 1:
			return "set " + describe_literal(a) + " to " + describe_literal(b);
		case 2:
			return "push " + describe_literal(a) + " to the stack";
		case 3:
			return "pop to " + describe_literal(a);
		case 4:
			return "" + describe_literal(a) + " = " + describe_literal(b) + " == " + describe_literal(c);
		case 5:
			return "gt " + describe_literal(a) + " = " + describe_literal(b) + " > " + describe_literal(c);
		case 6:
			return "jump to " + describe_literal(a);
		case 7:
			return "jump if " + describe_literal(a) + ", to " + describe_literal(b);
			break;
		case 8:
			return "jump if !" + describe_literal(a) + ", to " + describe_literal(b);
		case 9:
			return "add into " + describe_literal(a) + ", " + describe_literal(b) + " + " + describe_literal(c);
		case 10:
			return "mult into " + describe_literal(a) + ", " + describe_literal(b) + " * " + describe_literal(c);
		case 11:
			return "mod into " + describe_literal(a) + ", " + describe_literal(b) + " mod " + describe_literal(c);
		case 12:
			return "and into " + describe_literal(a) + ", " + describe_literal(b) + " AND " + describe_literal(c);
		case 13:
			return "or into" + describe_literal(a) + ", " + describe_literal(b) + " OR " + describe_literal(c);
		case 14:
			return "not, " + describe_literal(a) + " = NOT " + describe_literal(b);
		case 15:
			return "rmem into " + describe_literal(a) + " from address at " + describe_literal(b);
		case 16:
			return "wmem into address at " + describe_literal(a) + " the value from " + describe_literal(b);
		case 17:
			return "call, write next instruction to stack and jump to " + describe_literal(a);
		case 18:
			return "ret, pop from stack and jump there";
		case 19:
			return "out " + describe_literal(a);
		case 20:
			return "in " + describe_literal(a);
		case 21:
			return "noop";
		default:
			break;
	}

	return "";
}

void VM::run()
{
	while(step() == true){}
}

void VM::set_input_stream(std::istream* new_stream)
{
	this->instream = new_stream;
}


bool VM::is_halted()
{
	return halted;
}

bool VM::requests_input()
{
	return memory[program_counter] == 20;
}

uint16_t VM::get_register(uint16_t register_number)
{
	if(register_number > 7)
	{
		throw "Invalid register number";
	}

	return registers[register_number];
}

void VM::set_register(uint16_t register_number, uint16_t value)
{
	if(register_number > 7)
	{
		throw "Invalid register number";
	}

	registers[register_number] = value;
}

uint16_t VM::get_memory(uint16_t memory_address)
{
	if(memory_address > 32768)
	{
		throw "Invalid memory address";
	}

	return memory[memory_address];
}

uint16_t VM::get_program_counter()
{
	return program_counter;
}

void VM::set_program_counter(uint16_t pc)
{
	program_counter = pc;
}


std::vector<uint16_t> VM::get_stack()
{
	std::stack<uint16_t> copy = this->stack;
	std::vector<uint16_t> values;

	while(! copy.empty())
	{
		values.push_back(copy.top());
		copy.pop();
	}

	return values;
}

uint16_t VM::literal_value(uint16_t parameter)
{
	if(parameter >= 32768 && parameter <= 32775) return registers[parameter - 32768];
	else if(parameter < 32768) return parameter;
	
	throw std::string("Invalid memory address " + std::to_string(parameter));
}

void VM::store_in(uint16_t address, uint16_t value)
{
	if(address >= 32768 && address <= 32775) registers[address - 32768] = value;
	else if(address < 32768) memory[address] = value;
	else throw std::string("Invalid memory address " + std::to_string(address));
}