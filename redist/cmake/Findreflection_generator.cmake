# Looks for reflection generator
#
# 1. Looks for 'reflection_generator' binary. Uses REFLECTOR_GENERATOR_BIN if set
# 2. defines function generate_reflection(SOURCE TARGET)

if(NOT ${REFLECTION_GENERATOR_EXECUTABLE})
	find_program(REFLECTION_GENERATOR_EXECUTABLE reflection_generator)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(reflection_generator REQUIRED_VARS REFLECTION_GENERATOR_EXECUTABLE)

message(STATUS "Using reflection generator: ${REFLECTION_GENERATOR_EXECUTABLE}")

function(generate_reflection SOURCE_FILE TARGET_FILE)
	set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_FILE}")
	set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")

	add_custom_command(
		OUTPUT ${OUTPUT_FILE}
		COMMAND ${REFLECTION_GENERATOR_EXECUTABLE}
		ARGS -p=${CMAKE_BINARY_DIR} -o=${OUTPUT_FILE} ${INPUT_FILE}
		MAIN_DEPENDENCY ${INPUT_FILE}
		DEPENDS ${REFLECTION_GENERATOR_EXECUTABLE})
	set_source_files_properties(${SOURCE_FILE} PROPERTIES LANG CXX)

endfunction()
