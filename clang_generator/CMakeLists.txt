set(LLVM_USED_LIBS clangTooling)

add_clang_executable(reflection_generator

	main.cpp
)

target_link_libraries(reflection_generator
	clangTooling
)


