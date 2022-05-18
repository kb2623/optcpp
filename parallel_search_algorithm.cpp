#include "parallel_search_algorithm.hpp"

tuple<double, vector<double>> ParallelSearchAlgorithm::run(TestFuncBounds* func) {
    initRun(func);
    sync = new Barrier(no_thr);
    auto workers = std::vector<std::thread>();
    for (int i = 1; i < no_thr; i++) workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this, i));
    run_thread(0);
    for (int i = 0; i < workers.size(); i++) workers[i].join();
    delete sync;
    return std::make_tuple(f_best, x_best);
}

void ParallelSearchAlgorithm::run_thread(int id) {
    while (!stop_cond(*this)) run_iteration(id);
}

void ParallelSearchAlgorithm::run_iteration() {
    throw "Multithread algorithm need and id for runiing (run_iteration)!!!";
}

void ParallelSearchAlgorithm::fix_solution(double *x, int id) {
    for (int i = 0; i < func->dim; i++) if (x[i] < func->x_bound_min[i] || x[i] > func->x_bound_max[i]) x[i] = func->x_bound_min[i] + (func->x_bound_max[i] - func->x_bound_min[i]) * randDouble(id);
}

void ParallelSearchAlgorithm::fix_solution(double *x) {
    throw "Multithread algorithm need and id for runiing (fix_solution)!!!";
}
