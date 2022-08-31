#include "search_algorithm.hpp"

#include "thread_data.hpp"

#include <limits>
#include <cstdlib>

// ----------------------- SearchAlgorithm -----------------------

template <typename T>
SearchAlgorithm<T>::SearchAlgorithm() : StoppingCondition(), _no_gen(0) {
	f_best = std::numeric_limits<double>::max();
	x_best = std::vector<double>();
	thread_td = new thread_data();
}

template <typename T>
SearchAlgorithm<T>::SearchAlgorithm(const SearchAlgorithm<T>& o) {}

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
	// One of the stopping conditinos predondition
	start_timer();
}

template <typename T>
tuple<double, vector<T>> SearchAlgorithm<T>::run(BoundedObjectiveFunction<T>* func) {
	initRun(func);
	while (!stop_cond(*this)) {
		run_iteration();
		// One of the stopping conditions precondition of counting
		_no_gen++;
	}
	return std::make_tuple(f_best, x_best);
}

template <typename T>
void SearchAlgorithm<T>::setParameters(AlgParams* params) {}

template <typename T>
size_t SearchAlgorithm<T>::rand() {
	return thread_td->dist(thread_td->prand);
}

template <typename T>
double SearchAlgorithm<T>::randDouble() {
	double r = rand();
	if (r == 0) return 0;
	else return r / double(std::numeric_limits<size_t>::max());
}

template <typename T>
inline unsigned long long int SearchAlgorithm<T>::no_gen() const {
	return _no_gen;
}

template <typename T>
bool SearchAlgorithm<T>::max_no_fes() {
	return fitf.no_fes() >= lim_no_fes;
}

template <typename T>
bool SearchAlgorithm<T>::max_no_gen() {
	return _no_gen >= lim_no_gen;
}

template <typename T>
bool SearchAlgorithm<T>::target_value() {
	return f_best <= fitness_target_value;
}

// ----------------------- ParallelSearchAlgorithm -----------------------

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm() : ParallelSearchAlgorithm(1, std::rand() % RAND_MAX) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr) : ParallelSearchAlgorithm(no_thr, std::rand() % RAND_MAX) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr, size_t seed) : SearchAlgorithm<T>() {
	this->no_thr = no_thr;
}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(const ParallelSearchAlgorithm<T>& o) {}

template <typename T>
ParallelSearchAlgorithm<T>::~ParallelSearchAlgorithm() {
	if (sync != nullptr) delete sync;
}

template <typename T>
void ParallelSearchAlgorithm<T>::setParameters(AlgParams* params) {
	SearchAlgorithm<T>::setParameters(params);
	this->no_thr = getParam(params, "no_thr", 1);
}

template <typename T>
tuple<double, vector<T>> ParallelSearchAlgorithm<T>::run(BoundedObjectiveFunction<T>* func) {
	initRun(func);
	sync = new Barrier(no_thr);
	auto workers = vector<std::thread>();
	for (int i = 1; i < no_thr; i++) workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this, i));
	run_thread(0);
	for (int i = 0; i < workers.size(); i++) workers[i].join();
	delete sync;
	sync = nullptr;
	return make_tuple(this->f_best, this->x_best);
}

template<typename T>
void ParallelSearchAlgorithm<T>::run_thread(size_t tid) {
	if (tid != 0) thread_td = new thread_data(tid);
	while (!stop_cond(*this)) {
		SearchAlgorithm<T>::run_iteration();
		if (thread_td->tid == 0) this->_no_gen++;
	}
	if (tid != 0) delete thread_td;
}
