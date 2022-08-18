#include"search_algorithm.hpp"

#include <limits>

SearchAlgorithm::SearchAlgorithm() {
	nfes = 0;
	f_best = std::numeric_limits<double>::max();
	x_best = std::vector<double>();
	stop_cond = &SearchAlgorithm::nfes_stop_cond;
}

SearchAlgorithm::~SearchAlgorithm() {}

// make new individual randomly
double* SearchAlgorithm::makeNewIndividual() {
	double* individual = new double[func->dim];
	for (int i = 0; i < func->dim; i++) individual[i] = ((func->x_bound_max[i] - func->x_bound_min[i]) * randDouble()) + func->x_bound_min[i];
	return individual;
}

void SearchAlgorithm::setBestSolution(double *x, double f) {
	best_lock.lock();
	if (f_best > f) {
		f_best = f;
		for (int i = 0; i < func->dim; i++) x_best[i] = x[i];
	}
	best_lock.unlock();
}

void SearchAlgorithm::initRun(TestFuncBounds *func) {
	nfes = 0;
	this->func = func;
	x_best = std::vector<double>(func->dim);
	f_best = std::numeric_limits<double>::max();
}

double SearchAlgorithm::eval(double *x) {
	double f;
	nfes++;
	func->test_func(x, &f, 1);
	return f;
}

size_t SearchAlgorithm::get_nfes() {
	return nfes;
}

void SearchAlgorithm::fix_solution(double *x) {
	for (int i = 0; i < func->dim; i++) if (x[i] < func->x_bound_min[i] || x[i] > func->x_bound_max[i]) x[i] = func->x_bound_min[i] + (func->x_bound_max[i] - func->x_bound_min[i]) * randDouble();
}

bool SearchAlgorithm::nfes_stop_cond() {
	return nfes >= func->max_num_evaluations;
}

tuple<double, vector<double>> SearchAlgorithm::run(TestFuncBounds *ifunc) {
	initRun(ifunc);
	while (!stop_cond(*this)) run_iteration();
	return std::make_tuple(f_best, x_best);
}

// No parameter algorithm
void SearchAlgorithm::setParameters(AlgParams *params) {}

double SearchAlgorithm::randDouble() {
	return (double)rand() / (double) RAND_MAX;
}

double SearchAlgorithm::cauchy_g(double mu, double gamma) {
	return mu + gamma * tan(M_PI * (randDouble() - 0.5));
}

double SearchAlgorithm::gauss(double mu, double sigma){
	return mu + sigma * sqrt(-2.0 * log(randDouble())) * sin(2.0 * M_PI * randDouble());
}

inline void copy_vector(double* src, double* dst, size_t size) {
	for (size_t i = 0; i < size; i++) dst[i] = src[i];
}
