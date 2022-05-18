#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "parallel_search_algorithm.hpp"

class Bar : public ParallelSearchAlgorithm {
public:
    size_t np;
    vector<vector<double>> x;
    vector<double> xf;

    Bar() : ParallelSearchAlgorithm() {
        np = 10;
    };

    Bar(size_t inp, size_t ino_thr = 10) : ParallelSearchAlgorithm(ino_thr) {
        this->np = inp;
    };

    virtual string info() override;
    virtual string sinfo() override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_iteration(int) override;
    void initRun(TestFuncBounds*) override;
    void start(int);
};

#endif
