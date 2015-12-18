#include "vm.hpp"

class Debugger
{
public:
	Debugger(VM vm);

	void run();

	 /* Will need:
		* Register inspection
		* PC breakpoints
		* Memory dump
		*/

private:
	VM vm;
	bool step_mode;

	bool should_step();
};