#include "parallel_search_algorithm.hpp"

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr, size_t seed) : SearchAlgorithm<T>() {
	this->no_thr = no_thr;
	for (int i = 0; i < no_thr - 1; i++) this->prand.push_back(std::default_random_engine(seed));
	for (int i = 0; i < no_thr - 1; i++) this->dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm(size_t no_thr) : ParallelSearchAlgorithm(no_thr, std::rand() % RAND_MAX) {}

template <typename T>
ParallelSearchAlgorithm<T>::ParallelSearchAlgorithm() : ParallelSearchAlgorithm(1, std::rand() % RAND_MAX) {}

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
	for (int i = 1; i < no_thr; i++) workers.emplace_back(std::thread(&ParallelSearchAlgorithm::run_thread, this));
	run_thread();
	for (int i = 0; i < workers.size(); i++) workers[i].join();
	delete sync;
	return make_tuple(this->f_best, this->x_best);
}

template<typename T>
void ParallelSearchAlgorithm<T>::run_thread() {
	int tid = thread_index();
	if (tid == -1) optcpp::tid = 0;
	else optcpp::tid = tid + 1;
	while (!stop_cond(*this)) {
		SearchAlgorithm<T>::run_iteration();
		if (optcpp::tid == 0) this->_no_gen++;
	}
}

template<typename T>
int ParallelSearchAlgorithm<T>::thread_index() {
	std::thread::id tid = std::this_thread::get_id();
	for (size_t i = 0; i < threads.size(); i++) if (tid == threads[i].get_id()) return i;
	return -1;
}

template<typename T>
size_t ParallelSearchAlgorithm<T>::rand() {
	return this->dists[optcpp::tid](this->prand[optcpp::tid]);
}
