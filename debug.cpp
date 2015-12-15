#include "debugger.hpp"

int main()
{
	Debugger debugger(VM("challenge.bin"));
	debugger.run();
	return 0;	
}