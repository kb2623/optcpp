/*
  L-SHADE implemented by C++ for Special Session & Competition on Real-Parameter Single Objective Optimization at CEC-2014
  See the details of L-SHADE in the following paper:

  * Ryoji Tanabe and Alex Fukunaga: Improving the Search Performance of SHADE Using Linear Population Size Reduction,  Proc. IEEE Congress on Evolutionary Computation (CEC-2014), Beijing, July, 2014.

  Version: 1.0   Date: 16/Apr/2014
  Written by Ryoji Tanabe (rt.ryoji.tanabe [at] gmail.com)
*/

#include"analize_algorithm.hpp"

#include <limits>

void AnalizeAlgorithm::initRun(TestFuncBounds *func) {
    nfes = 0;
    this->func = func;
}

double AnalizeAlgorithm::eval(double *x) {
    double f;
    func->test_func(x, &f, 1);
    nfes++;
    return f;
}
