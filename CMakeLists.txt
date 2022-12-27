cmake_minimum_required(VERSION 3.23)
project(FormalkiPrac2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)


add_executable(run
        main.cpp
        CYK/CYK_Manager.cpp
        CYK/CYK_Manager.hpp
        Grammar/src/CF_Grammar.cpp
        Grammar/src/Parser.cpp
        Grammar/src/ChomskyBuilder.cpp
        Grammar/include/CF_Grammar.h
        Grammar/include/Builder.hpp
        )

add_subdirectory(Tests)

include_directories(
        Grammar/include
        CYK
)

