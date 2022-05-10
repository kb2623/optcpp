#ifndef _RDG_H_
#define _RDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"

class RDG: public AnalizeAlgorithm {
public:
    double alpha = 10e-12;
    unsigned int np = 10;

    RDG() {}
    ~RDG() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    virtual vector<unsigned int> interact(double*, double, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);
    double calc_epsilon(TestFuncBounds* func);

};

vector<unsigned int> vunion(vector<unsigned int>, vector<unsigned int>);

#endif
