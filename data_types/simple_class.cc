#include "simple_class.hh"


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


}
