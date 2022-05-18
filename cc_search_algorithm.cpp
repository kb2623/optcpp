#include "cc_search_algorithm.hpp"

std::tuple<double, std::vector<double>> CooperativeCoevolution::run(TestFuncBounds* ifunc) {
    ParallelSearchAlgorithm::initRun(ifunc);
    return std::make_tuple(f_best, x_best);
}

