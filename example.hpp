#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <barrier>

#include "parallel_search_algorithm.hpp"

class Bar : public ParallelSearchAlgorithm {
public:
    size_t np;
    std::vector<std::vector<double>> x;
    std::vector<double> xf;

    Bar() : ParallelSearchAlgorithm() {
        np = 10;
    };

    Bar(size_t inp, size_t ino_thr = 10) : ParallelSearchAlgorithm(ino_thr) {
        this->np = inp;
    };

    virtual std::string info() override;
    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds*) override;
    virtual void run_thread(int) override;
    void initRun(TestFuncBounds*);
    void start(int);
};

#endif
