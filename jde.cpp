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

void jDE::run_thread(int id) {
    auto s = ceil(np / double(no_thr));
    auto y = new double[func->dim];
    while (nfes < func->max_num_evaluations) {
        for (int i = s * id; i < np && i < s * (id + 1); i++) {
            double F_n = F_l + rand(id) * F_u;
            Fs[i] = (rand(id) < tao_1) ? F_n : Fs[i];
            double CR_n = rand(id);
            CRs[i] = (rand(id) < tao_2) ? CR_n : CRs[i];
            auto f = rand_1(id, i, y);
            sync_point->arrive_and_wait();
            if (f < popf[i]) {
                for (int j = 0; j < func->dim; j++) pop[i][j] = y[j];
                popf[i] = f;
                setBestSolution(y, f);
            }
            sync_point->arrive_and_wait();
        }
    }
    delete [] y;
}
