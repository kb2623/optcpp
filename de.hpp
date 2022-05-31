#ifndef _DE_H_
#define _DE_H_

#include "parallel_search_algorithm.hpp"
#include "de_mutations.hpp"

using namespace std;

class DE: public ParallelSearchAlgorithm, public DeMutations {
public:
    DE();
    DE(size_t);
    DE(size_t, size_t);
    ~DE();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams*) override;
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

    size_t np;
    double F;
    double CR;
};

#endif
