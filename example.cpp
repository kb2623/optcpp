#include "example.hpp"

string Bar::info() {
    return "Testni algorithm";
}

string Bar::sinfo() {
    return "Bar";
}

void Bar::initRun(TestFuncBounds *ifunc) {
    SearchAlgorithm::initRun(ifunc);
    x = std::vector<std::vector<double>>(np, std::vector<double>(func->dim));
    xf = std::vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble(0) + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
}

tuple<double, vector<double>> Bar::run(TestFuncBounds *ifunc) {
    initRun(ifunc);
    auto r = ParallelSearchAlgorithm::run(ifunc);
    x.clear();
    xf.clear();
    return r;
}

void Bar::run_thread(int id) {
    int s = ceil(np / double(no_thr));
    while (nfes < func->max_num_evaluations) {
        for (int i = s * id; i < np && i < (id + 1) * s; i++) {
            for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble(id) + func->x_bound_min[j];
            xf[i] = eval(x[i].data());
        }
        sync->wait();
    }
};
