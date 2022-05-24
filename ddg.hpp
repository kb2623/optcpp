#ifndef _DDG_H_
#define _DDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"

#include <algorithm>

class DDG: public AnalizeAlgorithm {
public:
    DDG();
    ~DDG();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams*) override;
    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
    double get_epsilon_addi(double, double, double, double);

    double epsilon_multi;
    double epsilon_addi;
};

#endif
