#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fno-rtti")

set(LLVM_USED_LIBS clangTooling)

#set(LLVM_LINK_COMPONENTS
#    MCParser
#    Option
#    MC
#    TransformUtils
#    BitReader
#    Core
#    Support
#  )

add_clang_executable(reflection_generator

	main.cpp
)

target_link_libraries(reflection_generator
    clangTooling
)

#target_link_libraries(reflection_generator
#	clangFrontend
#	clangDriver
#	clangSerialization
#	clangParse
#	clangSema
#	clangEdit
#	clangLex
#	clangAnalysis
#	clangAST
#	clangBasic

#	LLVMMCParser
#	LLVMOption
#	LLVMMC
#	LLVMTransformUtils
#	LLVMBitReader
#	LLVMCore
#	LLVMSupport

#	tinfo
#	dl
#	pthread
#)


