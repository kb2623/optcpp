/*
  L-SHADE implemented by C++ for Special Session & Competition on Real-Parameter Single Objective Optimization at CEC-2014
  See the details of L-SHADE in the following paper:

  * Ryoji Tanabe and Alex Fukunaga: Improving the Search Performance of SHADE Using Linear Population Size Reduction,  Proc. IEEE Congress on Evolutionary Computation (CEC-2014), Beijing, July, 2014.

  Version: 1.0   Date: 16/Apr/2014
  Written by Ryoji Tanabe (rt.ryoji.tanabe [at] gmail.com)
*/

#include"jso.hpp"

// make new individual randomly
double* SearchAlgorithm::makeNewIndividual() {
    double* individual = new double[func->dim];
    for (int i = 0; i < func->dim; i++) individual[i] = ((func->x_bound_max[i] - func->x_bound_min[i]) * randDouble()) + func->x_bound_min[i];
    return individual;
}

void SearchAlgorithm::setBestSolution(double *x, double f) {
    if (f_best > f) {
        f_best = f;
        for (int i = 0; i < func->dim; i++) x_best[i] = x[i];
    }
}

void SearchAlgorithm::initRun(TestFuncBounds *func) {
    nfes = 0;
    this->func = func;
    x_best = vector<double>(func->dim);
    f_best = numeric_limits<double>::max();
}

double SearchAlgorithm::eval(double *x) {
    double f;
    func->test_func(x, &f, 1);
    setBestSolution(x, f);
    nfes++;
    return f;
}
