#include"de.hpp"

string DE::info() {
    string r = "Differential Evolution (DE)";
    if (no_thr > 1) r = "Parallel " + r;
    return r;
}

void DE::initRun(TestFuncBounds *ifunc) {
    SearchAlgorithm::initRun(ifunc);
    for (int i = 0; i < np; i++) {
        pop.push_back(makeNewIndividual());
        popf.push_back(eval(pop[i]));
    }
}

tuple<double, vector<double>> DE::run(TestFuncBounds *ifunc) {
    initRun(ifunc);
    auto r = ParallelSearchAlgorithm::run(ifunc);
    for (auto e : pop) delete [] e;
    pop.clear();
    popf.clear();
    return r;
}

void DE::run_thread(int id) {
    auto s = ceil(np / double(no_thr));
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


