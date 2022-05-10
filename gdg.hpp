#ifndef _GDG_H_
#define _GDG_H_

#include "analize_algorithm.hpp"

class GDG: public AnalizeAlgorithm {
public:
    GDG() {}
    ~GDG() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

};

#endif
