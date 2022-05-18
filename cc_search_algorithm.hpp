#ifndef _CC_SEARCH_ALGORITHM_
#define _CC_SEARCH_ALGORITHM_

#include "parallel_search_algorithm.hpp"
#include "analize_algorithm.hpp"

#include <random>
#include <thread>
#include "barrier.hpp"

class CooperativeCoevolution: public ParallelSearchAlgorithm {
public:
    CooperativeCoevolution() : ParallelSearchAlgorithm() {};

    CooperativeCoevolution(size_t no_thr) : ParallelSearchAlgorithm(no_thr) {};

    CooperativeCoevolution(size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed) {};

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;

private:
    vector<SearchAlgorithm*> algs;
    AnalizeAlgorithm* anal;
};

#endif
