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

void Bar::initRun(TestFuncBounds *ifunc) {
	SearchAlgorithm::initRun(ifunc);
	x = std::vector<std::vector<double>>(np, std::vector<double>(func->dim));
	xf = std::vector<double>(np);
	for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble(0) + func->x_bound_min[j];
	xf[0] = f_best = eval(x[0].data());
	for (int i = 0; i < func->dim; i++) x_best[i] = x[0][i];
	for (int i = 1; i < np; i++) {
		xf[i] = eval(x[i].data());
		if (xf[i] < f_best) for (int k = 0; k < func->dim; k++) x_best[k] = x[i][k];
	}
}

tuple<double, vector<double>> Bar::run(TestFuncBounds *ifunc) {
	auto r = ParallelSearchAlgorithm::run(ifunc);
	x.clear();
	xf.clear();
	return r;
}

void Bar::run_iteration(int id) {
	int s = ceil(np / double(no_thr));
	for (int i = s * id; i < np && i < (id + 1) * s; i++) {
		for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble(id) + func->x_bound_min[j];
		xf[i] = eval(x[i].data());
		if (f_best > xf[i]) {
			best_lock.lock();
			for (int k = 0; k < func->dim; k++) x_best[k] = x[i][k];
			f_best = xf[i];
			best_lock.unlock();
		}
	}
	sync->wait();
}

void Bar::setParameters(AlgParams *params) {
	ParallelSearchAlgorithm::setParameters(params);
	this->np = getParam(params, "np", 50);
}
