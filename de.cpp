#include"de.hpp"

#include <iostream>

DE::DE() : ParallelSearchAlgorithm<double>(1), DeMutations(), CooperativeCoevolutionOptimizer() {}

DE::DE(size_t no_thr) : ParallelSearchAlgorithm(no_thr), DeMutations(), CooperativeCoevolutionOptimizer() {}

DE::DE(size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed), DeMutations(), CooperativeCoevolutionOptimizer() {}

DE::~DE() {
	pop.clear();
	popf.clear();
}

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

void DE::setParameters(AlgParams* params) {
	ParallelSearchAlgorithm::setParameters(params);
	this->np = getParam(params, "np", 50);
	this->F  = getParam(params, "F",  0.9);
	this->CR = getParam(params, "CR", 0.9);
}

void DE::initRun(BoundedObjectiveFunction<double> *func) {
	SearchAlgorithm::initRun(func);
	for (int i = 0; i < np; i++) {
		pop.push_back(makeNewArrayIndividual());
		popf.push_back(fitf(pop[i]));
	}
}

tuple<double, vector<double>> DE::run(BoundedObjectiveFunction<double>* func) {
	auto r = ParallelSearchAlgorithm::run(func);
	for (auto e : pop) delete [] e;
	pop.clear();
	popf.clear();
	return r;
}

void DE::run_iteration() {
	auto s = ceil(np / double(no_thr));
	auto y = new double[fitf.dim()];
	for (int i = s * optcpp::tid; i < s * (optcpp::tid + 1); i++) if (i < np) {
		auto f = opt(*this, i, y);
		sync->arrive_and_wait();
		if (f < popf[i]) {
			for (int j = 0; j < fitf.dim(); j++) pop[i][j] = y[j];
			popf[i] = f;
			setBestSolution(y, f);
		}
		sync->arrive_and_wait();
	} else {
		sync->arrive_and_wait();
		sync->arrive_and_wait();
	}
	delete [] y;
}

double DE::rand_1(int i, double* y) {
	size_t a, b, c;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	int r = rand() % fitf.dim();
	for (int j = 0; j < fitf.dim(); j++) {
		if (randDouble() < CR || j == r) {
			y[j] = pop[a][j] + F * (pop[b][j] - pop[c][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return fitf(y);
}

double DE::best_2(int i, double *y) {
	int a, b, c, d;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	do d = rand() % np; while (d == i || d == b || d == a || d == c);
	int r = rand() % fitf.dim();
	for (int j = 0; j < fitf.dim(); j++) {
		if (randDouble() < CR || j == r) {
			y[j] = x_best[j] + F * (pop[a][j] - pop[b][j]) + F * (pop[c][j] - pop[d][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return fitf(y);
}

double DE::rand_to_best_1(int i, double *y) {
	int a, b, c;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	int r = rand() % fitf.dim();
	for (int j = 0; j < fitf.dim(); j++) {
		if (randDouble() < CR || j == r) {
			y[j] = pop[a][j] + F * (x_best[j] - pop[i][j]) + F * (pop[b][j] - pop[c][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return fitf(y);
}
