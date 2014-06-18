add_executable(fully_qualified_name

	main.cc
	fully_qualified_name_builder.hh
)


target_link_libraries(fully_qualified_name
	data_types
)
