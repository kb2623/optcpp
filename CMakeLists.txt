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
set(COMMON_SRC barrier.cpp common_funcs.cpp thread_data.cpp)
set(COMMON_HDR barrier.hpp common_funcs.hpp thread_data.hpp)
add_library(Common ${COMMON_SRC} ${COMMON_HDR})

# Interfaces
set(INTERFACES_SRC objective_function.cpp algorithm_parameters.cpp analize_algorithm.cpp search_algorithm.cpp stopping_condition.cpp cc_optimizer.cpp cc_search_algorithm.cpp repair_solution.cpp)
set(INTERFACES_HDR objective_function.hpp algorithm_parameters.hpp analize_algorithm.hpp search_algorithm.hpp stopping_condition.hpp cc_optimizer.hpp cc_search_algorithm.hpp repair_solution.hpp)
add_library(Interfaces ${INTERFACES_HDR} ${INTERFACES_SRC})
target_link_libraries(Interfaces PUBLIC Common)

# Benchmarks
set(BENCHMARK_SRC cec17_test_func.cpp sphere.cpp)
set(BENCHMARK_HDR cec17_test_func.hpp sphere.hpp)
add_library(Benchmarks ${BENCHMARK_HDR} ${BENCHMARK_SRC})
target_link_libraries(Benchmarks PUBLIC Interfaces)

# Implemented analyze algorithms
## set(ANA_ALGS_SRC xdg.cpp xdgv1.cpp xdgv2.cpp rdg.cpp rdgv2.cpp rdgv3.cpp dg.cpp ddg.cpp ddgv1.cpp ddgv2.cpp gdg.cpp gdgv1.cpp gdgv2.cpp fii.cpp)
set(ANA_ALGS_SRC xdg.cpp)
## set(ANA_ALGS_HDR xdg.hpp xdgv1.hpp xdgv2.hpp rdg.hpp rdgv2.hpp rdgv3.hpp dg.hpp ddg.hpp ddgv1.hpp ddgv2.hpp gdg.hpp gdgv1.hpp gdgv2.hpp fii.hpp)
set(ANA_ALGS_HDR xdg.hpp)
add_library(AlgsAnalyse ${ANA_ALGS_SRC} ${ANA_ALGS_HDR})
target_link_libraries(AlgsAnalyse PUBLIC Benchmarks)

# Implemented search algorithms
## set(OPT_ALGS_SRC example.cpp jso.cpp jsop.cpp de.cpp jde.cpp pso.cpp)
set(OPT_ALGS_SRC example.cpp)
## set(OPT_ALGS_HDR example.hpp jso.hpp jsop.hpp de.hpp jde.hpp de_mutations.hpp pso.hpp)
set(OPT_ALGS_HDR example.hpp)
add_library(AlgsOptimization ${OPT_ALGS_SRC} ${OPT_ALGS_HDR})
target_link_libraries(AlgsOptimization PUBLIC AlgsAnalyse)

# Main target
add_executable(OptCpp main.cpp)
target_link_libraries(OptCpp AlgsOptimization)
