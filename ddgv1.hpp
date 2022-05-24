#ifndef _DDGV1_H_
#define _DDGV1_H_

#include "ddg.hpp"

class DDGv1: public DDG {
public:
    DDGv1();
    ~DDGv1();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams*) override;
    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
    double get_epsilon_addi(double, double, double, double);
    double calc_epsilon();

    double alpha;
    unsigned int np = 50;
    double epsilon_addi = 0;
};

#endif
