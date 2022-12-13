cmake_minimum_required(VERSION 3.23)
project(FormalkiPrac2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)


add_executable(run
        main.cpp
        Grammar/CF_Grammar.cpp
        CYK/CYK_Manager.cpp
        CYK/CYK_Manager.hpp
        )

add_subdirectory(Tests)

include_directories(
        Grammar
        CYK
)

