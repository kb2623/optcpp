#ifndef _GDG_H_
#define _GDG_H_

#include "analize_algorithm.hpp"

class GDG: public AnalizeAlgorithm {
public:
    double epsilon;
    size_t np;

    GDG() : GDG(10) {};
    GDG(size_t np) : np(np) {};
    ~GDG() {};

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    void graph_connected_components();
    double calc_treshold();

};

#endif
