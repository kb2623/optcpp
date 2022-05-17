#ifndef _GDGV2_H_
#define _GDGV2_H_

#include "gdg.hpp"

class GDGv2: public GDG {
public:
    GDGv2() : GDG(10, 10e-10) {};
    GDGv2(size_t np, double epsilon) : GDG(np, epsilon) {};
    ~GDGv2() {};

    virtual string info() override;
    virtual string sinfo() override;

protected:
    double calc_treshold(vector<vector<double>>&);
};

#endif
