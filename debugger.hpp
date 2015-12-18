#include <set>
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
	std::set<uint16_t> breakpoints;

	bool should_step();
};