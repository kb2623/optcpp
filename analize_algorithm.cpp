#include"analize_algorithm.hpp"

template <typename T>
AnalizeAlgorithm<T>::AnalizeAlgorithm() : prand(), dists() {
	prand.push_back(std::default_random_engine());
	dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
}

template <typename T>
AnalizeAlgorithm<T>::~AnalizeAlgorithm() {}

template <typename T>
void AnalizeAlgorithm<T>::initRun(BoundedObjectiveFunction<T>* func) {
	this->func = func;
}

template <typename T>
void AnalizeAlgorithm<T>::setParameters(AlgParams *params) {}

template <typename T>
size_t AnalizeAlgorithm<T>::rand() {
	return dists[0](prand[0]);
}

template <typename T>
double AnalizeAlgorithm<T>::randDouble() {
	double r = rand();
	if (r == 0) return 0;
	else return rand() / std::numeric_limits<size_t>::max();
}
