#include <iostream>
#include <stdint.h>
#include <string>
#include <stack>
#include <fstream>
#include <vector>

class VM
{
	friend class Debugger;
public:
	VM();
	VM(std::string binary_name);

	// Returns false if halted
	bool step() {return step(false);};
	bool step(bool continue_until_input);
	bool is_halted();
	bool requests_input();
	std::string describe_instruction();
	std::string describe_instruction(uint16_t program_counter);

	// Runs until halted
	void run();

	void set_input_stream(std::istream* new_stream);

protected:
	uint16_t get_register(uint16_t register_number);
	void set_register(uint16_t register_number, uint16_t value);
	uint16_t get_memory(uint16_t memory_address);
	uint16_t get_program_counter();
	void set_program_counter(uint16_t pc);
	std::vector<uint16_t> get_stack();

private:
	uint16_t memory[32768];
	uint16_t registers[8];
	std::stack<uint16_t> stack;
	uint16_t program_counter;
	bool halted;
	std::istream* instream;
	std::string describe_literal(uint16_t literal);

	uint16_t literal_value(uint16_t parameter);
	void store_in(uint16_t address, uint16_t value);
};