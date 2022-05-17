#ifndef _GDGV1_H_
#define _GDGV1_H_

#include "gdg.hpp"

class GDGv1: public GDG {
public:
    GDGv1() : GDG(10, 10e-10) {};
    GDGv1(size_t np, double epsilon) : GDG(np, epsilon) {};
    ~GDGv1() {};

    virtual string info() override;
    virtual string sinfo() override;

protected:
    double calc_treshold(vector<vector<double>>&);
};

#endif
