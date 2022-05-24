#ifndef _CC_SEARCH_ALGORITHM_
#define _CC_SEARCH_ALGORITHM_

#include "parallel_search_algorithm.hpp"
#include "analize_algorithm.hpp"

#include <random>
#include <thread>
#include "barrier.hpp"

class CooperativeCoevolution: public ParallelSearchAlgorithm {
public:
    CooperativeCoevolution() : CooperativeCoevolution(vector<SearchAlgorithm*>(1, nullptr), nullptr, 1, 1) {};

    CooperativeCoevolution(size_t no_thr) : CooperativeCoevolution(vector<SearchAlgorithm*>(1, nullptr), nullptr, no_thr, 1) {};

    CooperativeCoevolution(size_t no_thr, size_t seed) : CooperativeCoevolution(vector<SearchAlgorithm*>(1, nullptr), nullptr, no_thr, seed) {};

    CooperativeCoevolution(vector<SearchAlgorithm*> algs, AnalizeAlgorithm *anal, size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed), algs(algs), anal(anal) {};

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;
    virtual void run_iteration(int) override;
    virtual void run_thread(int) override;

private:
    vector<SearchAlgorithm*> algs;
    AnalizeAlgorithm* anal;
};

#endif
