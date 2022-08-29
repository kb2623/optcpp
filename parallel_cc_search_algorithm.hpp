#ifndef _PARALLEL_CC_SEARCH_ALGORITHM_H_
#define _PARALLEL_CC_SEARCH_ALGORITHM_H_

#include "parallel_search_algorithm.hpp"
#include "analize_algorithm.hpp"

template<typename T>
class ParallelCooperativeCoevolution : public ParallelSearchAlgorithm<T> {
public:
	ParallelCooperativeCoevolution();
	ParallelCooperativeCoevolution(size_t no_thr);
	ParallelCooperativeCoevolution(size_t no_thr, size_t seed);
	~ParallelCooperativeCoevolution();

	/**
	 * @brief run_thread
	 */
	virtual void run_thread() override;
	/**
	 * @brief run
	 * @param problem
	 * @return
	 */
	virtual std::tuple<double, std::vector<T>> run(BoundedObjectiveFunction<T>* problem) override;

private:
	vector<SearchAlgorithm<T>*> algs;
	AnalizeAlgorithm<T>* anal;
};

#endif
