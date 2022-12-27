cmake_minimum_required(VERSION 3.6)
project(gtest)

enable_testing()

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
		../Grammar/include
		../CYK
)

add_subdirectory(
        lib/googletest
)

add_executable(basic_tests
		../Grammar/src/CF_Grammar.cpp
        ../CYK/CYK_Manager.hpp
        ../CYK/CYK_Manager.cpp
		../Grammar/src/Parser.cpp
		../Grammar/src/ChomskyBuilder.cpp
		Balanced_tests.cpp
		psp_tests.cpp
		Regexes_tests.cpp
		2ndFromHwTest.cpp
		diff_psp_tests.cpp
		fromCW_test.cpp
		)
add_executable(inside_tests
		../Grammar/src/CF_Grammar.cpp
		../CYK/CYK_Manager.hpp
		../CYK/CYK_Manager.cpp
		../Grammar/src/Parser.cpp
		../Grammar/src/ChomskyBuilder.cpp
		inside_tests/test_builder.cpp
		inside_tests/test_parser.cpp
	    )

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(basic_tests gtest gtest_main)
target_link_libraries(inside_tests gtest gtest_main)
