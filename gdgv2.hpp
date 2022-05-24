#ifndef _GDGV2_H_
#define _GDGV2_H_

#include "gdg.hpp"

class GDGv2: public GDG {
public:
    GDGv2();
    GDGv2(size_t, double);
    ~GDGv2();

    virtual string info() override;
    virtual string sinfo() override;

protected:
    double calc_treshold(vector<vector<double>>&);
};

#endif
