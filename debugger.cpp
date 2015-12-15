#include "debugger.hpp"

Debugger::Debugger(VM vm) : step_mode(true)
{
	this->vm = vm;
}

void Debugger::run()
{
	while(! vm.is_halted())
	{
		vm.step();

		if(should_step())
		{
			// Insert prompt here
		}
	}
}

bool Debugger::should_step()
{
	return step_mode;
}