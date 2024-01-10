cmake_minimum_required(VERSION 3.5)

project(OptCpp LANGUAGES CXX)

# Set project to standard C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build flags based on release type
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# FIXME: Better support for threads
# Support for threads in linux
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Copy needed input files for running bechmarks
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Common target
add_library(Common common_funcs.hpp)
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)

# Interfaces
add_library(Interfaces algorithm.hpp objective_function.hpp)
target_link_libraries(Interfaces PUBLIC Common)
set_target_properties(Interfaces PROPERTIES LINKER_LANGUAGE CXX)

# Implemented analyze algorithms
set(ANA_ALGS_SRC xdg.cpp xdgv1.cpp xdgv2.cpp rdg.cpp rdgv2.cpp rdgv3.cpp dg.cpp ddg.cpp ddgv1.cpp ddgv2.cpp gdg.cpp gdgv1.cpp gdgv2.cpp fii.cpp)
set(ANA_ALGS_HDR xdg.hpp xdgv1.hpp xdgv2.hpp rdg.hpp rdgv2.hpp rdgv3.hpp dg.hpp ddg.hpp ddgv1.hpp ddgv2.hpp gdg.hpp gdgv1.hpp gdgv2.hpp fii.hpp)
add_library(AlgsAnalyse ${ANA_ALGS_SRC} ${ANA_ALGS_HDR})
target_link_libraries(AlgsAnalyse PUBLIC Interfaces)

# Benchmarks
set(BENCHMARK_SRC sphere.cpp cec17_test_func.cpp)
set(BENCHMARK_HDR sphere.hpp cec17_test_func.hpp)
add_library(Benchmarks ${BENCHMARK_HDR} ${BENCHMARK_SRC})
target_link_libraries(Benchmarks PUBLIC AlgsAnalyse)

# Main target
add_executable(OptCpp main.cpp)
target_link_libraries(OptCpp PUBLIC Benchmarks)
