#ifndef _XDGV2_H_
#define _XDGV2_H_

#include "xdg.hpp"
#include "testfunc.hpp"

#include <algorithm>

class XDGv2: public XDG {
public:
    double eps = 10e-12;

    XDGv2() {}
    ~XDGv2() {}

    virtual string info() override;
    virtual string sinfo() override;

protected:
    double epsilon(double, double, double, double);
    double gamma(double);

};

#endif
