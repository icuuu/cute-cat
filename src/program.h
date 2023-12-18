#pragma once

#include <string>

#include "context.h"

class Program
{
public:
	Program(const char* configPath);

	void run();

	Context context;
};
