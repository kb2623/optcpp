#ifndef _ANALIZE_ALGORITHM_H_
#define _ANALIZE_ALGORITHM_H_

#include "testfunc.hpp"
#include "algorithm_parameters.hpp"

#include <limits>
#include <atomic>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>

using std::string;
using std::tuple;
using std::vector;

class AnalizeAlgorithm {
public:
    AnalizeAlgorithm(AlgParams);
    AnalizeAlgorithm();
    ~AnalizeAlgorithm();

    virtual string info() = 0;
    virtual string sinfo() = 0;
    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) = 0;

    virtual void setParameters(AlgParams&);

protected:
    void initRun(TestFuncBounds* func);
    double eval(double* x);

    //Return random value with uniform distribution [0, 1)
    inline double randDouble() {
        return double(rand()) / double(RAND_MAX);
    }

    /*
      Return random value from Cauchy distribution with mean "mu" and variance "gamma"
      http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Cauchy
      */
    inline double cauchy_g(double mu, double gamma) {
        return mu + gamma * tan(M_PI * (randDouble() - 0.5));
    }

    /*
      Return random value from normal distribution with mean "mu" and variance "gamma"
      http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Gauss
      */
    inline double gauss(double mu, double sigma){
        return mu + sigma * sqrt(-2.0 * log(randDouble())) * sin(2.0 * M_PI * randDouble());
    }

    std::atomic<unsigned int> nfes;
    TestFuncBounds* func;
};

#endif
