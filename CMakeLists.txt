cmake_minimum_required(VERSION 3.5)

project(OptCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(OptCpp main.cpp barrier.cpp barrier.hpp analize_algorithm.cpp analize_algorithm.hpp testfunc.hpp cec17_test_func.cpp cec17_test_func.hpp sphere.cpp sphere.hpp search_algorithm.cpp search_algorithm.hpp parallel_search_algorithm.cpp parallel_search_algorithm.hpp example.cpp example.hpp jso.cpp jso.hpp jsop.cpp jsop.hpp de.cpp de.hpp jde.cpp jde.hpp xdg.cpp xdg.hpp xdgv1.cpp xdgv1.hpp xdgv2.cpp xdgv2.hpp rdg.cpp rdg.hpp rdgv2.cpp rdgv2.hpp rdgv3.cpp rdgv3.hpp dg.cpp dg.hpp ddg.cpp ddg.hpp ddgv1.cpp ddgv1.hpp ddgv2.cpp ddgv2.hpp gdg.cpp gdg.hpp)
