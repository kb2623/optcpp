#include "example.hpp"

Bar::Bar() : ParallelSearchAlgorithm() {}

Bar::Bar(size_t no_thr) : ParallelSearchAlgorithm(no_thr) {}

Bar::Bar(size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed) {}

Bar::~Bar() {}

string Bar::info() {
	return "Testni algorithm";
}

string Bar::sinfo() {
	return "Bar";
}

void Bar::initRun(BoundedObjectiveFunction<double>* func) {
	SearchAlgorithm::initRun(func);
	x = std::vector<std::vector<double>>(np, std::vector<double>(fitf.dim()));
	xf = std::vector<double>(np);
	for (int i = 0; i < np; i++) x[i] = makeNewVectorIndividual();
	xf[0] = f_best = fitf(x[0].data());
	for (int i = 0; i < fitf.dim(); i++) x_best[i] = x[0][i];
	for (int i = 1; i < np; i++) {
		xf[i] = fitf(x[i].data());
		if (xf[i] < f_best) for (int k = 0; k < fitf.dim(); k++) x_best[k] = x[i][k];
	}
}

tuple<double, vector<double>> Bar::run(BoundedObjectiveFunction<double>* func) {
	auto r = ParallelSearchAlgorithm::run(func);
	x.clear();
	xf.clear();
	return r;
}

void Bar::run_iteration() {
	int s = ceil(np / double(no_thr));
	for (int i = s * optcpp::tid; i < np && i < (optcpp::tid + 1) * s; i++) {
		for (int j = 0; j < fitf.dim(); j++) x[i][j] = fitf.x_bound_max(j) - fitf.x_bound_min(j) * randDouble() + fitf.x_bound_min(j);
		xf[i] = fitf(x[i].data());
		if (f_best > xf[i]) {
			best_lock.lock();
			for (int k = 0; k < fitf.dim(); k++) x_best[k] = x[i][k];
			f_best = xf[i];
			best_lock.unlock();
		}
	}
	sync->arrive_and_wait();
}

void Bar::setParameters(AlgParams *params) {
	ParallelSearchAlgorithm::setParameters(params);
	this->np = getParam(params, "np", 50);
}
