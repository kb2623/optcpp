#ifndef _PARALLEL_CC_SEARCH_ALGORITHM_H_
#define _PARALLEL_CC_SEARCH_ALGORITHM_H_

#include "parallel_search_algorithm.hpp"
#include "analize_algorithm.hpp"

#include <random>
#include <thread>
#include "barrier.hpp"

class ParallelCooperativeCoevolution : public ParallelSearchAlgorithm {
public:
    ParallelCooperativeCoevolution();
    ParallelCooperativeCoevolution(size_t no_thr);
    ParallelCooperativeCoevolution(size_t no_thr, size_t seed);
    ~ParallelCooperativeCoevolution();

    virtual void run_iteration(int) = 0;

    virtual void run_thread(int) override;
    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;

private:
    vector<SearchAlgorithm*> algs;
    AnalizeAlgorithm* anal;
};

#endif
