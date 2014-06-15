#include "simple_class.hh"

#include <iostream>

namespace ProjectNamespace
{


SimpleClass::SimpleClass()
	: mCounter(0)
{

}

void SimpleClass::AddToCounter(unsigned value)
{
	mCounter += value;
}

void SimpleClass::SetName(const std::string& name, int number)
{
	std::cout << "SetName(" << name << ", " << number << ") called" << std::endl;
	mName = name + std::to_string(number);
}


}
