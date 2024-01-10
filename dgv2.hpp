#ifndef _DGV2_H_
#define _DGV2_H_

#include "dg.hpp"

#include <algorithm>

class DGv2: public DG {
public:
    double eps = 10e-12;

    DGv2();
    DGv2(const DGv2&);
    ~DGv2();

    virtual std::string info();

protected:
    double epsilon(double, double, double, double);
    double gamma(double);

};

#endif
