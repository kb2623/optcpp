#ifndef _PARALLEL_SEARCH_ALGORITHM_H_
#define _PARALLEL_SEARCH_ALGORITHM_H_

#include "search_algorithm.hpp"
#include "barrier.hpp"

#include <random>

namespace optcpp {
	thread_local int tid = 0;
}

template<typename T>
class ParallelSearchAlgorithm: public SearchAlgorithm<T>  {
public:
	ParallelSearchAlgorithm();
	ParallelSearchAlgorithm(size_t);
	ParallelSearchAlgorithm(size_t, size_t);

	/**
	 * @brief run_thread
	 */
	virtual void run_thread();
	/**
	 * @brief run
	 * @param fun
	 * @return
	 */
	virtual std::tuple<double, std::vector<T>> run(BoundedObjectiveFunction<T>* fun) override;
	/**
	 * @brief setParameters
	 * @param params
	 */
	virtual void setParameters(AlgParams* params) override;

protected:
	/**
	 * @brief thread_index
	 * @return
	 */
	int thread_index();
	/**
	 * @brief rand
	 * @return
	 */
	size_t rand() override;

protected:
	size_t no_thr;

protected:
	Barrier* sync;
	vector<std::thread> threads;
};

#endif
