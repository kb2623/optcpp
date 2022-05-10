#ifndef _DDGV1_H_
#define _DDGV1_H_

#include "ddg.hpp"

class DDGv1: public DDG {
public:
    double alpha = 10e-12;
    unsigned int np = 50;
    double epsilon_addi = 0;

    DDGv1() {}
    ~DDGv1() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    double get_epsilon_addi(double, double, double, double);
    double calc_epsilon();

};

#endif
