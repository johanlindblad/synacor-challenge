#include <iostream>
#include <stdint.h>
#include <string>
#include <stack>
#include <fstream>
#include "vm.hpp"

int main()
{
	VM vm("challenge.bin");
	vm.run();
	return 0;
}

