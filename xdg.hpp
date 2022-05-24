#ifndef _XDG_H_
#define _XDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"

#include <algorithm>

class XDG: public AnalizeAlgorithm {
public:
    XDG();
    ~XDG();

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams&) override;
    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
    double epsilon(double, double, double, double);

    double _epsilon;
};

#endif
