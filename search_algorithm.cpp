#include "search_algorithm.hpp"

#include "thread_data.hpp"

#include <limits>
#include <cstdlib>

// ----------------------- SearchAlgorithm -----------------------

template <typename T>
SearchAlgorithm<T>::SearchAlgorithm() {}

template <typename T>
SearchAlgorithm<T>::SearchAlgorithm(const SearchAlgorithm<T>& o) {}

template <typename T>
SearchAlgorithm<T>::~SearchAlgorithm() {}

template <typename T>
RunAlgParams<T>& SearchAlgorithm<T>::initRun(thread_data& tdata, BoundedObjectiveFunction<T>& func) {
	RunAlgParams<T> run_params;
	// One of the stopping conditinos predondition
	run_params.start_timer();
	return run_params;
}

template <typename T>
tuple<double, vector<T>> SearchAlgorithm<T>::run(BoundedObjectiveFunction<T>& func) {
	RunAlgParams<T> params = initRun(func);
	thread_data tdata = thread_data();
	while (!stop_cond(*this)) {
		run_iteration(tdata, params);
		// Increment an algorithm generation
		params++;
	}
	return params.getBestSolution();
}

template <typename T>
void SearchAlgorithm<T>::setParameters(AlgParams& params) {}

// ----------------------- ParallelSearchAlgorithm -----------------------

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm() : ParallelSearchAlgorithm(1, std::rand() % RAND_MAX) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr) : ParallelSearchAlgorithm(no_thr, std::rand() % RAND_MAX) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr, size_t seed) : SearchAlgorithm<T>(), no_thr(no_thr) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(const ParallelSearchAlgorithm<T>& o) {}

template <typename T>
ParallelSearchAlgorithm<T>::~ParallelSearchAlgorithm() {}

template <typename T>
void ParallelSearchAlgorithm<T>::setParameters(AlgParams& params) {
	SearchAlgorithm<T>::setParameters(params);
	this->no_thr = getParam(params, "no_thr", 1);
}

template <typename T>
RunParallelAlgParams<T>& ParallelSearchAlgorithm<T>::initRun(thread_data& tdata, BoundedObjectiveFunction<T>& func) {
	auto tbarrier = Barrier(no_thr);
	auto params = RunParallelAlgParams<T>(tbarrier);
	// TODO start timer
	return params;
}

template <typename T>
tuple<double, vector<T>> ParallelSearchAlgorithm<T>::run(BoundedObjectiveFunction<T>& func) {
	auto tdata_main = thread_data(0);
	auto params = initRun(tdata_main, func);
	auto workers = vector<std::thread>();
	for (int i = 1; i < no_thr; i++) {
		thread_data tdata = thread_data(i);
		workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this, tdata, params));
	}
	run_thread(tdata_main, params);
	for (int i = 0; i < workers.size(); i++) workers[i].join();
	return params.getBestSolution();
}

template <typename T>
void ParallelSearchAlgorithm<T>::run_thread(thread_data& tdata, RunParallelAlgParams<T>& params) {
	while (!stop_cond(*this)) {
		run_iteration(tdata, params);
		// Increment an algorithm generation if main thread
		params.arrive_and_wait();
		if (tdata.tid == 0) params++;
	}
}
