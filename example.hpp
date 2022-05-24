#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "parallel_search_algorithm.hpp"

class Bar : public ParallelSearchAlgorithm {
public:
    Bar();
    Bar(size_t, size_t);
    ~Bar();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams&) override;
    void initRun(TestFuncBounds*) override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_iteration(int) override;

protected:
    size_t np;
    vector<double> xf;
    vector<vector<double>> x;
};

#endif
