#include"jde.hpp"

string jDE::info() {
    return " Brest, J., Greiner, S., Boskovic, B., Mernik, M. and Zumer, V. (2006) Self-Adapting Control Parameters in Differential Evolution: A Comparative Study on Numerical Benchmark Problems. IEEE Transactions on Evolutionary Computation, 10, 646-657. https://doi.org/10.1109/TEVC.2006.872133 ";
}

string jDE::sinfo() {
    string r = "jDE";
    if (no_thr > 1) r = r + 'p';
    return r;
}

void jDE::initRun(TestFuncBounds *ifunc) {
    DE::initRun(ifunc);
    for (int i = 0; i < np; i++) {
        Fs.push_back(F_s);
        CRs.push_back(CR_s);
    }
}

tuple<double, vector<double>> jDE::run(TestFuncBounds *ifunc) {
    initRun(ifunc);
    auto r = DE::run(ifunc);
    for (auto e : pop) delete [] e;
    Fs.clear();
    CRs.clear();
    return r;
}

void jDE::run_iteration(int id) {
    auto s = ceil(np / double(no_thr));
    auto y = new double[func->dim];
    for (int i = s * id; i < np && i < s * (id + 1); i++) {
        double F_n = F_l + rand(id) * F_u;
        Fs[i] = (rand(id) < tao_1) ? F_n : Fs[i];
        double CR_n = rand(id);
        CRs[i] = (rand(id) < tao_2) ? CR_n : CRs[i];
        auto f = opt(*this, id, i, y);
        sync->wait();
        if (f < popf[i]) {
            for (int j = 0; j < func->dim; j++) pop[i][j] = y[j];
            popf[i] = f;
            setBestSolution(y, f);
        }
        sync->wait();
    }
    delete [] y;
}

double jDE::rand_1(int id, int i, double* y) {
    size_t a, b, c;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
        if (randDouble(id) < CRs[i] || j == r) {
            y[j] = pop[a][j] + Fs[i] * (pop[b][j] - pop[c][j]);
        } else {
            y[j] = pop[i][j];
        }
    }
    fix_solution(y, id);
    return eval(y);
}

double jDE::best_2(int id, int i, double *y) {
    int a, b, c, d;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    do d = rand(id) % np; while (d == i || d == b || d == a || d == c);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
        if (randDouble(id) < CRs[i] || j == r) {
            y[j] = x_best[j] + Fs[i] * (pop[a][j] - pop[b][j]) + F * (pop[c][j] - pop[d][j]);
        } else {
            y[j] = pop[i][j];
        }
    }
    fix_solution(y, id);
    return eval(y);
}

double jDE::rand_to_best_1(int id, int i, double *y) {
    int a, b, c;
    do a = rand(id) % np; while (a == i);
    do b = rand(id) % np; while (b == i || b == a);
    do c = rand(id) % np; while (c == i || c == b || c == a);
    int r = rand(id) % func->dim;
    for (int j = 0; j < func->dim; j++) {
        if (randDouble(id) < CRs[i] || j == r) {
            y[j] = pop[a][j] + Fs[i] * (x_best[j] - pop[i][j]) + F * (pop[b][j] - pop[c][j]);
        } else {
            y[j] = pop[i][j];
        }
    }
    fix_solution(y, id);
    return eval(y);
}
