#include "data_types/reflection.hh"

#include "sql_generator.hh"

#include <iostream>
#include <vector>

int main()
{
	std::cout << "Hello!" << std::endl;

	std::cout << "SQL table defintion for record1: " << std::endl;

	sql::generate_table_definition<record1>(std::cout, "table_of_record1");

	std::cout << "SQL table defintion for record2: " << std::endl;
	sql::generate_enum_definion<::some_enum>(std::cout);
	sql::generate_table_definition<record2>(std::cout, "table_of_record2");

	std::cout << "SQL insert statement for collection of records" << std::endl;

	std::vector<::record1> col = { {1, 1.1, "one"}, {2, 2.2, "two"}};

	sql::generate_insert(std::cout, "table_of_record1", col);

}
