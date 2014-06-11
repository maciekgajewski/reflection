#include <iostream>

#include "data_types/reflection.hh"

#include "sql_generator.hh"

int main()
{
	std::cout << "Hello!" << std::endl;

	std::cout << "SQL table defintion for record1: " << std::endl;

	sql::generate_table_definition<record1>(std::cout, "table_of_record1");

	std::cout << "SQL table defintion for record2: " << std::endl;
	sql::generate_enum_definion<::some_enum>(std::cout);
	sql::generate_table_definition<record2>(std::cout, "table_of_record2");

}
