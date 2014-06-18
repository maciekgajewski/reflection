#include "data_types/reflection.hh"

#include "fully_qualified_name_builder.hh"

#include <iostream>
int main()
{
	using meta = reflection::meta_object<ProjectNamespace::SimpleClass>;

	std::cout << "fully-qualified name is: ";
	build_fully_qualifed_name<meta>(std::cout);
	std::cout << std::endl;
}
