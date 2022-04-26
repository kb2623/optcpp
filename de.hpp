#ifndef _DE_H_
#define _DE_H_

#include <vector>
#include <cmath>
#include <limits>
#include <barrier>

#include "search_algorithm.hpp"

using namespace std;

class DE: public SearchAlgorithm {
public:
    size_t no_thr;
    std::barrier<> *sync_point;

    size_t np;
    std::vector<double *> pop;
    std::vector<double> popf;

    double F;
    double CR;

    DE() {
        no_thr = 1;
        np = 50;
        F = 0.9;
        CR = 0.65;
    };

    ~DE() {};

    virtual std::string info();
    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds*);
    void initRun(TestFuncBounds*);
    void evolve(int);

};

#endif
