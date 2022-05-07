#include "parallel_search_algorithm.hpp"

std::tuple<double, std::vector<double>> ParallelSearchAlgorithm::run(TestFuncBounds* func) {
    initRun(func);
    sync = new Barrier(no_thr);
    auto workers = std::vector<std::thread>();
    for (int i = 1; i < no_thr; i++) workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this, i));
    run_thread(0);
    for (int i = 0; i < workers.size(); i++) workers[i].join();
    delete sync;
    return std::make_tuple(f_best, x_best);
}

