#ifndef _SEARCH_ALGORITHM_H_
#define _SEARCH_ALGORITHM_H_

#include "testfunc.hpp"

#include <limits>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
#include <atomic>

class SearchAlgorithm {
public:
    SearchAlgorithm() {
        nfes = 0;
        f_best = std::numeric_limits<double>::max();
        x_best = std::vector<double>();
    }
    ~SearchAlgorithm() {}

    virtual std::string info() = 0;
    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds*) = 0;

    std::vector<double> x_best;
    double  f_best;

protected:
    void initRun(TestFuncBounds* func);
    double* makeNewIndividual();
    double eval(double* x);
    void setBestSolution(double*, double);

    //Return random value with uniform distribution [0, 1)
    inline double randDouble() {
        return (double)rand() / (double) RAND_MAX;
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