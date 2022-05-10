#ifndef _DG_H_
#define _DG_H_

#include "analize_algorithm.hpp"

class DG: public AnalizeAlgorithm {
public:
    DG() {}
    ~DG() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    double epsilon(double, double, double, double);

};

#endif
