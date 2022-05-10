#ifndef _XDGV1_H_
#define _XDGV1_H_

#include "xdg.hpp"
#include "testfunc.hpp"

#include <algorithm>

class XDGv1: public XDG {
public:
    double alpha = 10e-12;
    unsigned int np = 50;
    double _epsilon;

    XDGv1() {}
    ~XDGv1() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    double epsilon(double, double, double, double);
    double calc_epsilon();

};

#endif
