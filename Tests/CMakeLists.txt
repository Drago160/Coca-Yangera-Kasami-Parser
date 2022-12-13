cmake_minimum_required(VERSION 3.6)
project(gtest)

enable_testing()

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ../CYK
        ../Grammar
)




add_subdirectory(
        lib/googletest
)

add_executable(BasicTests
        ../Grammar/CF_Grammar.cpp
        ../CYK/CYK_Manager.hpp
        ../CYK/CYK_Manager.cpp
        Balanced_tests.cpp
        psp_tests.cpp
        Regexes_tests.cpp
        2ndFromHwTest.cpp
        diff_psp_tests.cpp
            )



set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(BasicTests gtest gtest_main)

