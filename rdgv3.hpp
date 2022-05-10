#ifndef _RDGV3_H_
#define _RDGV3_H_

#include "rdgv2.hpp"
#include "testfunc.hpp"

class RDGv3: public RDGv2 {
public:
    double tnf = 0.1;

    RDGv3() {}
    ~RDGv3() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    vector<unsigned int> interact(double*, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);

};

#endif
