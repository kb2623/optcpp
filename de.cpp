#include"de.hpp"

std::string DE::info() {
    return "TODO";
}

void DE::initRun(TestFuncBounds *ifunc) {
    SearchAlgorithm::initRun(ifunc);
    for (int i = 0; i < np; i++) {
        pop.push_back(makeNewIndividual());
        popf.push_back(eval(pop[i]));
    }
}

std::tuple<double, std::vector<double>> DE::run(TestFuncBounds* func) {
    initRun(func);
    sync_point = new std::barrier(no_thr);
    auto threads = std::vector<std::thread>();
    for (int i = 0; i < no_thr - 1; i++) threads.emplace_back(std::thread(&DE::evolve, this, i + 1));
    evolve(0);
    for (int i = 0; i < no_thr - 1; i++) threads[i].join();
    for (int i = 0; i < np; i++) delete [] pop[i];
    pop.clear();
    popf.clear();
    return std::make_tuple(f_best, x_best);
}

void DE::evolve(int id) {
    auto s = np / no_thr;
    auto y = new double[func->dim];
    while (nfes < func->max_num_evaluations) {
        for (int i = s * id; i < np && i < s * (id + 1); i++) {
            int a, b, c;
            do a = rand() % np; while (a != i);
            do b = rand() % np; while (b != i && b != a);
            do c = rand() % np; while (c != i && c != b && c != a);
            int r = rand() % func->dim;
            for (int j = 0; j < func->dim; j++) {
                if (randDouble() < CR || j == r) {
                    y[j] = pop[a][j] + F * (pop[b][j] - pop[c][j]);
                } else {
                    y[j] = pop[i][j];
                }
            }
            auto f = eval(y);
            sync_point->arrive_and_wait();
            if (f < popf[i]) {
                for (int j = 0; j < func->dim; j++) pop[i][j] = y[j];
                popf[i] = f;
            }
            sync_point->arrive_and_wait();
        }
    }
    delete [] y;
}


