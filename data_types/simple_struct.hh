#pragma once

#include <string>
#include <cstdint>

struct record1
{
	int number;
	double angle;
	std::string name;
};


enum some_enum
{
	Alpha = 0,
	Beta = 3,
	Gamma = 7,
};

struct record2
{
	std::int64_t size;
	std::string location;
	some_enum type;
};
