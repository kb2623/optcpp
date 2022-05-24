#include"analize_algorithm.hpp"

AnalizeAlgorithm::AnalizeAlgorithm(AlgParams params) {
    nfes = 0;
    setParameters(params);
}

AnalizeAlgorithm::AnalizeAlgorithm() : AnalizeAlgorithm(AlgParams()) {}

AnalizeAlgorithm::~AnalizeAlgorithm() {}

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

/**
 * @brief AnalizeAlgorithm::setParameters No parameter algorithm.
 * @param params
 */
void AnalizeAlgorithm::setParameters(AlgParams &params) {}
