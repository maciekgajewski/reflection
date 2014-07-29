include_directories(/usr/lib/llvm-3.5/include)
#link_directories(/usr/lib/llvm-3.5/lib)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fno-rtti")

add_executable(clang_generator

	main.cpp
)

target_link_libraries(clang_generator
	clangFrontend
	clangDriver
	clangSerialization
	clangParse
	clangSema
	clangEdit
	clangLex
	clangAnalysis
	clangAST
	clangBasic

	LLVMMCParser
	LLVMOption
	LLVMMC
	LLVMTransformUtils
	LLVMBitReader
	LLVMCore
	LLVMSupport

	tinfo
	dl
	pthread
)


