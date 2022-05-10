#ifndef _DGV1_H_
#define _DGV1_H_

#include "dg.hpp"

#include <algorithm>

class DGv1: public DG {
public:
    double alpha = 10e-12;
    unsigned int np = 50;
    double _epsilon = 0;

    DGv1() {}
    ~DGv1() {}

    virtual std::tuple<std::vector<unsigned int>, std::vector<std::vector<unsigned int>>> run(TestFuncBounds*);
    virtual std::string info();

protected:
    double epsilon(double, double, double, double);
    double calc_epsilon();

};

#endif
