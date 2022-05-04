#ifndef _PARALLEL_SEARCH_ALGORITHM_
#define _PARALLEL_SEARCH_ALGORITHM_

#include "search_algorithm.hpp"

#include <barrier>

class ParallelSearchAlgorithm: public SearchAlgorithm {
public:
    ParallelSearchAlgorithm() : SearchAlgorithm() {
        no_thr = 2;
    }

    ParallelSearchAlgorithm(size_t no_thr) : SearchAlgorithm() {
        this->no_thr = no_thr;
    }

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;
    virtual void run_thread(int id) = 0;

protected:
    size_t no_thr;
    std::barrier<> *sync_point;
};

#endif
