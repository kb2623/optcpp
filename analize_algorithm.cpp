#include"analize_algorithm.hpp"

AnalizeAlgorithm::AnalizeAlgorithm() : nfes(0) {}

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

void AnalizeAlgorithm::setParameters(AlgParams *params) {
	/**
	  * Parameter free algorithms can use this function.
	  */
}
