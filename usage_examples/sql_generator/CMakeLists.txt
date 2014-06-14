add_executable(sql_generator

	main.cc
	sql_generator.hh
)


target_link_libraries(sql_generator
	data_types
)
