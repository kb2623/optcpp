#include "pso.hpp"

PSO::PSO() : ContinuousParallelSearchAlgorithm(1), CooperativeCoevolutionOptimizer() {}

PSO::PSO(size_t no_thr) : ContinuousParallelSearchAlgorithm(no_thr), CooperativeCoevolutionOptimizer() {}

PSO::PSO(size_t no_thr, size_t seed) : ContinuousParallelSearchAlgorithm(no_thr, seed), CooperativeCoevolutionOptimizer() {}

PSO::~PSO() {
	pop.clear();
	popf.clear();
	lpop.clear();
	lpopf.clear();
	V.clear();
	W.clear();
}

string PSO::info() {
	string r = "Particle Swarm Optimization (PSO)";
	if (no_thr > 1) r = "Parallel " + r;
	return r;
}

string PSO::sinfo() {
	string r = "PSO";
	if (no_thr > 1) r = r + 'p';
	return r;
}

void PSO::setParameters(AlgParams *params) {
	this->np = params != nullptr && params->has("np") ? params->at<size_t>("np") : 50;
	this->C1 = params != nullptr && params->has("C1") ? params->at<double>("C1") : 0.9;
	this->C2 = params != nullptr && params->has("C2") ? params->at<double>("C2") : 0.9;
	// TODO Add missing parameters
	//this->W = params != nullptr && params->has("W") ? params->at<
}

void PSO::initRun(TestFuncBounds<double>* func) {
	ParallelSearchAlgorithm::initRun(func);
	for (int i = 0; i < np; i++) {
		pop.push_back(makeNewIndividual());
		popf.push_back(eval(pop[i]));
	}
	for (int i = 0; i < pop.size(); i++) {
		double* x = new double[fitf->dim];
		for (int j = 0; j < fitf->dim; j++) x[j] = pop[i][j];
		lpop.push_back(x);
		lpopf.push_back(popf[i]);
	}
}

tuple<double, vector<double>> PSO::run(TestFuncBounds<double>* func) {
	auto r = ParallelSearchAlgorithm::run(func);
	for (auto e : pop) delete [] e;
	pop.clear(), popf.clear();
	lpop.clear(), lpopf.clear();
	V.clear();
	return r;
}

void PSO::run_iteration() {
	auto s = ceil(np / double(no_thr));
	auto v = new double[fitf->dim];
	for (int i = s * optcpp::tid; i < s * (optcpp::tid + 1); i++) if (i < np) {
		for (int j = 0; j < fitf->dim; j++) V[i][j] = W[j] * V[i][j] + C1 * rand() * (lpop[i][j] - pop[i][j]) + C2 * rand() * (x_best[j] - pop[i][j]);
		// TODO fix V to vMin and vMax
		for (int j = 0; j < fitf->dim; j++) pop[i][j] = pop[i][j] + V[i][j];
		// TODO fix pop to min and max of the problem
		// TODO eval pop
		// TODO check if new pop better than old lpop and if so update lpop and lpopf
		sync->arrive_and_wait();
		// TODO find new global best solution
		sync->arrive_and_wait();
	} else {
		sync->arrive_and_wait();
		sync->arrive_and_wait();
	}
	delete [] v;
}
