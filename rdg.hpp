#ifndef _RDG_H_
#define _RDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"


class RDG: public AnalizeAlgorithm {
public:
    RDG();
    ~RDG();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams&) override;
    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
    virtual vector<unsigned int> interact(double*, double, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);
    double calc_epsilon(TestFuncBounds* func);

    size_t np;
    double alpha;
};

#endif
