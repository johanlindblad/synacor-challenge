#include "vm.hpp"

VM::VM()
{
	std::fill_n(memory, 32768, 0);
	std::fill_n(registers, 8, 0);
};

VM::VM(std::string binary_name)
{
	std::ifstream f("challenge.bin", std::ios::in | std::ios::binary);
	f.read((char*) memory, 32768 * sizeof(uint16_t));

	std::fill_n(registers, 8, 0);
};

bool VM::step()
{
	if(halted) return false;

	uint16_t byte = memory[program_counter];

	// Potentially unsafe but will only be used after safe reads
	uint16_t a = memory[(program_counter + 1) % 32768];
	uint16_t b = memory[(program_counter + 2) % 32768];
	uint16_t c = memory[(program_counter + 3) % 32768];

	char character;

	switch(byte)
	{
		case 0:
			std::cout << "halt" << std::endl;
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
			character = std::cin.get();
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

	return true;
}

void VM::run()
{
	while(step() == true){}
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