#ifndef _DE_H_
#define _DE_H_

#include "parallel_search_algorithm.hpp"
#include "de_mutations.hpp"

using namespace std;

class DE: public ParallelSearchAlgorithm, public DeMutations {
public:
    size_t np;
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

    DE(size_t np, double F, double CR, size_t no_thr) : ParallelSearchAlgorithm(no_thr) {
        this->np = np;
        this->F = F;
        this->CR = CR;
    };

    DE(size_t np, double F, double CR, size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed) {
        this->np = np;
        this->F = F;
        this->CR = CR;
    };

    ~DE() {
        pop.clear();
        popf.clear();
    };

    virtual string info() override;
    virtual string sinfo() override;
    void initRun(TestFuncBounds*) override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_iteration(int) override;

    virtual double rand_1(int, int, double*) override;
    virtual double best_2(int, int, double*) override;
    virtual double rand_to_best_1(int, int, double*) override;

protected:
    vector<double*> pop;
    vector<double> popf;
    std::function<double(DE&, int, int, double*)> opt = &DE::rand_1;

};

#endif
