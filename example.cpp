#include "example.hpp"

// -------------------- BarParams --------------------

template <typename T>
BarParams<T>::BarParams() {}

template <typename T>
BarParams<T>::~BarParams() {}

// -------------------- Bar --------------------

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

RunParallelAlgParams<double>& Bar::initRun(BoundedObjectiveFunction<double>& func) {
	ParallelSearchAlgorithm::initRun(func);
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

void Bar::run_iteration(thread_data& tdata, RunParallelAlgParams<double>& params) {
	int s = ceil(np / double(no_thr));
	for (int i = s * tdata.tid; i < np && i < (tdata.tid + 1) * s; i++) {
		for (int j = 0; j < params.fitf().dim(); j++) x[i][j] = params.fitf().x_bound_max(j) - params.fitf().x_bound_min(j) * tdata.randDouble() + params.fitf().x_bound_min(j);
		xf[i] = params.fitf()(x[i].data());
		params.setBestSolution(x[i], xf[i])
	}
	params.arrive_and_wait();
}

void Bar::setParameters(AlgParams& params) {
	ParallelSearchAlgorithm::setParameters(params);
	this->np = getParam(params, "np", 50);
}
