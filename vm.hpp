#include <iostream>
#include <stdint.h>
#include <string>
#include <stack>
#include <fstream>

class VM
{
public:
	VM();
	VM(std::string binary_name);

	// Returns false if halted
	bool step();
	bool is_halted();

	// Runs until halted
	void run();

private:
	uint16_t memory[32768];
	uint16_t registers[8];
	std::stack<uint16_t> stack;
	uint16_t program_counter;
	bool halted;

	uint16_t literal_value(uint16_t parameter);
	void store_in(uint16_t address, uint16_t value);
};