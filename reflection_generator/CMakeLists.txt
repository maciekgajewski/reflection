set(LLVM_USED_LIBS clangTooling)

add_clang_executable(reflection_generator

	main.cpp

	main_output.hpp main_output.cpp
	ast_visitor.hpp
)

target_link_libraries(reflection_generator
	clangTooling
)


