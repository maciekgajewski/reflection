#pragma once

#include <string>

namespace ProjectNamespace
{

class SimpleClass
{
public:

	SimpleClass();

	void AddToCounter(unsigned value);
	void SetName(const std::string& name, int number) { mName = name + std::to_string(number); }

	unsigned GetCounter() const { return mCounter; }
	std::string GetName() const { return mName; }

private:

	unsigned mCounter;
	std::string mName;
};

}

