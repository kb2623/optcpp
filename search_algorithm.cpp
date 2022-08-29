#include"search_algorithm.hpp"

#include <limits>
#include <cstdlib>

template <typename T>
SearchAlgorithm<T>::SearchAlgorithm() : prand(), dists(), _no_gen(0) {
	f_best = std::numeric_limits<double>::max();
	x_best = std::vector<double>();
	prand.push_back(std::default_random_engine());
	dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
	// FIXME do something with stopping condition
}

template <typename T>
SearchAlgorithm<T>::~SearchAlgorithm() {}

template <typename T>
T* SearchAlgorithm<T>::makeNewArrayIndividual() {
	double* x = new T[fitf->dim];
	for (int i = 0; i < fitf->dim; i++) x[i] = (*fitf)[i];
	return x;
}

template <typename T>
vector<T> SearchAlgorithm<T>::makeNewVectorIndividual() {
	vector<T> x;
	for (int i = 0; i < fitf->dim; i++) x.push_back((*fitf)[i]);
	return x;
}

template <typename T>
void SearchAlgorithm<T>::setBestSolution(T* x, double f) {
	best_lock.lock();
	if (f_best > f) {
		f_best = f;
		for (int i = 0; i < fitf->dim; i++) x_best[i] = x[i];
	}
	best_lock.unlock();
}

template <typename T>
void SearchAlgorithm<T>::initRun(BoundedObjectiveFunction<T>* func) {
	this->fitf = func;
	this->_no_gen = 0;
	x_best = std::vector<double>(fitf->dim);
	f_best = std::numeric_limits<double>::max();
}

template <typename T>
tuple<double, vector<T>> SearchAlgorithm<T>::run(BoundedObjectiveFunction<T>* func) {
	initRun(func);
	while (!stop_cond(*this)) {
		run_iteration();
		_no_gen++;
	}
	return std::make_tuple(f_best, x_best);
}

template <typename T>
void SearchAlgorithm<T>::setParameters(AlgParams* params) {}

template <typename T>
size_t SearchAlgorithm<T>::rand() {
	return dists[0](prand[0]);
}

template <typename T>
double SearchAlgorithm<T>::randDouble() {
	double r = rand();
	if (r == 0) return 0;
	else return rand() / std::numeric_limits<size_t>::max();
}

template <typename T>
inline unsigned long long int SearchAlgorithm<T>::no_gen() const {
	return _no_gen;
}
