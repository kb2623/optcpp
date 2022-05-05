#ifndef _DE_H_
#define _DE_H_

#include "parallel_search_algorithm.hpp"

using namespace std;

class DE: public ParallelSearchAlgorithm {
public:
    size_t np;
    vector<double*> pop;
    vector<double> popf;

    double F;
    double CR;

    DE() : ParallelSearchAlgorithm() {
        np = 50;
        F = 0.9;
        CR = 0.65;
    };

    DE(size_t np, double F, double CR) : DE() {
        this->np = np;
        this->F = F;
        this->CR = CR;
    };

    DE(size_t np, double F, double CR, size_t no_thr) : DE(np, F, CR) {
        this->no_thr = no_thr;
    };

    ~DE() {
        pop.clear();
        popf.clear();
    };

    virtual string info() override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_thread(int) override;
    void initRun(TestFuncBounds*) override;

};

#endif
