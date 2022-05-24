#include "parallel_search_algorithm.hpp"

ParallelSearchAlgorithm::ParallelSearchAlgorithm(size_t no_thr, size_t seed) : SearchAlgorithm() {
     this->no_thr = no_thr;
     prand = vector<std::default_random_engine>();
     for (int i = 0; i < no_thr; i++) prand.push_back(std::default_random_engine(seed));
     dists = vector<std::uniform_int_distribution<size_t>>();
     for (int i = 0; i < no_thr; i++) dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
}

ParallelSearchAlgorithm::ParallelSearchAlgorithm(size_t no_thr) : ParallelSearchAlgorithm(2, std::rand() % RAND_MAX) {}

ParallelSearchAlgorithm::ParallelSearchAlgorithm() : ParallelSearchAlgorithm(2, 1) {}

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

void ParallelSearchAlgorithm::fix_solution(double *x, int id) {
    for (int i = 0; i < func->dim; i++) if (x[i] < func->x_bound_min[i] || x[i] > func->x_bound_max[i]) x[i] = func->x_bound_min[i] + (func->x_bound_max[i] - func->x_bound_min[i]) * randDouble(id);
}

void ParallelSearchAlgorithm::run_iteration() {
    throw "Multithread algorithm need and id for runiing (run_iteration)!!!";
}

void ParallelSearchAlgorithm::fix_solution(double *x) {
    throw "Multithread algorithm need and id for runiing (fix_solution)!!!";
}

size_t ParallelSearchAlgorithm::rand(int id) {
    return dists[id](prand[id]);
}

double ParallelSearchAlgorithm::randDouble(int id) {
    return double(rand(id)) / double(std::numeric_limits<size_t>::max());
}

double ParallelSearchAlgorithm::cauchy_g(int id, double mu, double gamma) {
    return mu + gamma * tan(M_PI * (randDouble(id) - 0.5));
}

double ParallelSearchAlgorithm::gauss(int id, double mu, double sigma){
    return mu + sigma * sqrt(-2.0 * log(randDouble(id))) * sin(2.0 * M_PI * randDouble(id));
}
