#pragma once

namespace ProjectNamespace
{

class SimpleClass
{
public:

	SimpleClass();

	void AddToCounter(unsigned value);
	unsigned GetCounter() const { return mCounter; }

private:

	unsigned mCounter;
};

}

