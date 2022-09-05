#include"analize_algorithm.hpp"

template <typename T>
AnalizeAlgorithm<T>::AnalizeAlgorithm() {}

template <typename T>
AnalizeAlgorithm<T>::AnalizeAlgorithm(const AnalizeAlgorithm& o) {}

template <typename T>
AnalizeAlgorithm<T>::~AnalizeAlgorithm() {}

template <typename T>
void AnalizeAlgorithm<T>::setParameters(AlgParams& params) {}

template <typename T>
RunAlgParams<T>& AnalizeAlgorithm<T>::initRun(thread_data& tdata, BoundedObjectiveFunction<T>& fitf) {
	return RunAlgParams(fitf);
}

template <typename T>
void AnalizeAlgorithm<T>::run_iteration(thread_data& tdata, RunAlgParams<T>& params) {}
