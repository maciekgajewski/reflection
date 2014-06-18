#pragma once

#include <ostream>


template<typename T>
void build_fully_qualifed_name(std::ostream& o)
{
	build_fully_qualifed_name<typename T::parent_namespace>(o);
	o << "::" << T::name;
}

template<>
void build_fully_qualifed_name<reflection::root_namespace>(std::ostream& o)
{
}
