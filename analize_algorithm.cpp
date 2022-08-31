#include"analize_algorithm.hpp"

template <typename T>
AnalizeAlgorithm<T>::AnalizeAlgorithm() {}

template <typename T>
AnalizeAlgorithm<T>::AnalizeAlgorithm(const AnalizeAlgorithm& o) {}

template <typename T>
AnalizeAlgorithm<T>::~AnalizeAlgorithm() {}

template <typename T>
void AnalizeAlgorithm<T>::initRun(BoundedObjectiveFunction<T>* func) {
	this->func = func;
}

template <typename T>
void AnalizeAlgorithm<T>::setParameters(AlgParams *params) {}
