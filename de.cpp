#include"de.hpp"

#include <iostream>

string DE::info() {
    string r = "Differential Evolution (DE)";
    if (no_thr > 1) r = "Parallel " + r;
    return r;
}

string DE::sinfo() {
    string r = "DE";
    if (no_thr > 1) r = r + 'p';
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
            auto f = opt(*this, id, i, y);
            sync->wait();
            if (f < popf[i]) {
                for (int j = 0; j < func->dim; j++) pop[i][j] = y[j];
                popf[i] = f;
                setBestSolution(y, f);
            }
            sync->wait();
        }
    }
    delete [] y;
}

double DE::rand_1(int id, int i, double* y) {
    size_t a, b, c;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
        if (randDouble(id) < CR || j == r) {
            y[j] = pop[a][j] + F * (pop[b][j] - pop[c][j]);
        } else {
            y[j] = pop[i][j];
        }
    }
    return eval(y);
}

double DE::best_2(int id, int i, double *y) {
    int a, b, c, d;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    do d = rand(id) % np; while (d == i || d == b || d == a || d == c);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
         if (randDouble(id) < CR || j == r) {
             y[j] = x_best[j] + F * (pop[a][j] - pop[b][j]) + F * (pop[c][j] - pop[d][j]);
         } else {
             y[j] = pop[i][j];
         }
     }
    return eval(y);
}

double DE::rand_to_best_1(int id, int i, double *y) {
    int a, b, c;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
        if (randDouble(id) < CR || j == r) {
            y[j] = pop[a][j] + F * (x_best[j] - pop[i][j]) + F * (pop[b][j] - pop[c][j]);
        } else {
            y[j] = pop[i][j];
        }
    }
    return eval(y);
}
