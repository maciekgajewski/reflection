find_package(reflection_generator REQUIRED)

include_directories(${CMAKE_BINARY_DIR}) # to make generated files available

#generate_reflection(enums.cpp enums_ref.hxx)
add_library(example_reflected_lib

	enums.cpp enums.hpp
	#${CMAKE_CURRENT_BINARY_DIR}/enums_ref.hxx
)

