cmake_minimum_required(VERSION 3.5)

project(OptCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(OptCpp main.cpp testfunc.hpp cec17_test_func.cpp cec17_test_func.hpp search_algorithm.cpp search_algorithm.hpp jso.cpp jso.hpp jsop.cpp jsop.hpp de.cpp de.hpp)
