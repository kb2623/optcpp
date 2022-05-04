#include "parallel_search_algorithm.hpp"

std::tuple<double, std::vector<double>> ParallelSearchAlgorithm::run(TestFuncBounds* func) {
    initRun(func);
    sync_point = new std::barrier(no_thr);
    auto workers = std::vector<std::thread>();
    for (int i = 1; i < no_thr; i++) workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this, i + 1));
    run_thread(0);
    for (int i = 0; i < workers.size(); i++) workers[i].join();
    delete sync_point;
    return std::make_tuple(f_best, x_best);
}

