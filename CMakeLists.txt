cmake_minimum_required(VERSION 3.5)

project(OptCpp LANGUAGES CXX)

# Set project to standard C++17
set(CMAKE_CXX_STANDARD 17)
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
set(COMMON_SRC barrier.cpp)
set(COMMON_HDR barrier.hpp)
add_library(Common ${COMMON_SRC} ${COMMON_HDR})

# Benchmark target
set(BENCMARK_SRC cec17_test_func.cpp sphere.cpp)
set(BENCMARK_HDR testfunc.hpp cec17_test_func.hpp sphere.hpp)
add_library(Benchmarks ${BENCMARK_SRC} ${BENCMARK_HDR})
target_link_libraries(Benchmarks PUBLIC Common)

# Analize algorithm target
set(ANA_ALGS_SRC analize_algorithm.cpp xdg.cpp xdgv1.cpp xdgv2.cpp rdg.cpp rdgv2.cpp rdgv3.cpp dg.cpp ddg.cpp ddgv1.cpp ddgv2.cpp gdg.cpp)
set(ANA_ALGS_HDR analize_algorithm.hpp xdg.hpp xdgv1.hpp xdgv2.hpp rdg.hpp rdgv2.hpp rdgv3.hpp dg.hpp ddg.hpp ddgv1.hpp ddgv2.hpp gdg.hpp)
add_library(AnaAlgs ${ANA_ALGS_SRC} ${ANA_ALGS_HDR})
target_link_libraries(AnaAlgs PUBLIC Benchmarks)

# Optimization algorithm target
set(OPT_ALGS_SRC search_algorithm.cpp parallel_search_algorithm.cpp example.cpp jso.cpp jsop.cpp de.cpp jde.cpp)
set(OPT_ALGS_HDR search_algorithm.hpp parallel_search_algorithm.hpp example.hpp jso.hpp jsop.hpp de.hpp jde.hpp)
add_library(OptAlgs ${OPT_ALGS_SRC} ${OPT_ALGS_HDR})
target_link_libraries(OptAlgs PUBLIC AnaAlgs)

# Main target
add_executable(OptCpp main.cpp)
target_link_libraries(OptCpp OptAlgs)
