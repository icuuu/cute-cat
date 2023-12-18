#include <iostream>
#include <string>

#include "program.h"

int main(int argc, char* argv[])
{	
	Program program(argc > 1 ? argv[1] : "config.json");
	program.run();
	
	return EXIT_SUCCESS;
}
