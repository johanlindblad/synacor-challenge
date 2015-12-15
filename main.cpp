#include <iostream>
#include <stdint.h>
#include <string>
#include <stack>
#include <fstream>

uint16_t memory[32768];
uint16_t registers[8];
std::stack<uint16_t> stack;
uint16_t program_counter = 0;

void store_in(uint16_t program_counter, uint16_t value)
{
	if(program_counter >= 32768 && program_counter <= 32775)
	{
		registers[program_counter - 32768] = value;
	}
	else if(program_counter < 32768)
	{
		memory[program_counter] = value;
	}
	else
	{
		throw std::string("Invalid memory program_counter " + std::to_string(program_counter));
	}
}

uint16_t value_of(uint16_t program_counter)
{
	if(program_counter >= 32768 && program_counter <= 32775)
	{
		return registers[program_counter - 32768];
	}
	else if(program_counter < 32768)
	{
		return program_counter;
	}
	else
	{
		throw std::string("Invalid memory program_counter " + std::to_string(program_counter));
	}
}

void write_to_console(uint16_t what)
{
	char value = value_of(what);
	std::cout << value;
}

int main()
{
	std::ifstream f("challenge.bin", std::ios::in | std::ios::binary);
	f.read((char*) memory, 32768 * sizeof(uint16_t));

	while(program_counter < 32768)
	{
		uint16_t byte = memory[program_counter];

		// Potentially unsafe
		uint16_t a = memory[(program_counter + 1) % 32768];
		uint16_t b = memory[(program_counter + 2) % 32768];
		uint16_t c = memory[(program_counter + 3) % 32768];

		char character;

		switch(byte)
		{
			case 0:
				goto halt;
				break;
			case 1:
				a -= 32768;
				registers[a] = value_of(b);

				program_counter += 3;
				break;
			case 2:
				stack.push(value_of(a));

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
				store_in(a, (value_of(b) == value_of(c)) ? 1 : 0);

				program_counter += 4;
				break;
			case 5:
				store_in(a, (value_of(b) > value_of(c)) ? 1 : 0);

				program_counter += 4;
				break;
			case 6:
				program_counter = a;
				break;
			case 7:
				program_counter += 3;

				if(value_of(a) > 0) program_counter = value_of(b);

				break;
			case 8:
				program_counter += 3;

				if(value_of(a) == 0) program_counter = value_of(b);

				break;
			case 9:
				store_in(a, (value_of(b) + value_of(c)) % 32768);

				program_counter += 4;
				break;
			case 10:
				store_in(a, (value_of(b) * value_of(c)) % 32768);

				program_counter += 4;
				break;
			case 11:
				store_in(a, (value_of(b) % value_of(c)));

				program_counter += 4;
				break;
			case 12:
				store_in(a, value_of(b) & value_of(c));

				program_counter += 4;
				break;
			case 13:
				store_in(a, value_of(b) | value_of(c));

				program_counter += 4;
				break;
			case 14:
				store_in(a, ~value_of(b) & ~(1 << 15));

				program_counter += 3;
				break;
			case 15:
			  b = memory[value_of(b)];
				store_in(a, b);

				program_counter += 3;
				break;
			case 16:
				memory[value_of(a)] = value_of(b);

				program_counter += 3;
				break;
			case 17:
				program_counter += 2;

				stack.push(program_counter);
				program_counter = value_of(a);

				break;
			case 18:
				program_counter = stack.top();
				stack.pop();
				break;
			case 19:
				write_to_console(value_of(a));

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
				return 1;
				break;
		}
	}

halt:

	return 0;
}

