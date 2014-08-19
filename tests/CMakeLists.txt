find_package(Boost 1.50 COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DBOOST_TEST_DYN_LINK)
add_executable(tests
	main.cpp

)

target_link_libraries(tests
	example_reflected_lib

	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

