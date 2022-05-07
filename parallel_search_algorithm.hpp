#ifndef _PARALLEL_SEARCH_ALGORITHM_
#define _PARALLEL_SEARCH_ALGORITHM_

#include "search_algorithm.hpp"

#include <random>
#include <thread>
#include "barrier.hpp"

class ParallelSearchAlgorithm: public SearchAlgorithm {
public:
    ParallelSearchAlgorithm() : SearchAlgorithm() {
        no_thr = 2;
        prand = vector<std::default_random_engine>();
        for (int i = 0; i < no_thr; i++) prand.push_back(std::default_random_engine(std::rand()));
        dists = vector<std::uniform_int_distribution<size_t>>();
        for (int i = 0; i < no_thr; i++) dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
    };

    ParallelSearchAlgorithm(size_t no_thr) : SearchAlgorithm() {
        this->no_thr = no_thr;
        prand = vector<std::default_random_engine>();
        for (int i = 0; i < no_thr; i++) prand.push_back(std::default_random_engine(std::rand()));
        dists = vector<std::uniform_int_distribution<size_t>>();
        for (int i = 0; i < no_thr; i++) dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));
    };

    ParallelSearchAlgorithm(size_t no_thr, size_t seed) : SearchAlgorithm() {
        this->no_thr = no_thr;
        prand = vector<std::default_random_engine>();
        for (int i = 0; i < no_thr; i++) prand.push_back(std::default_random_engine(seed));
        dists = vector<std::uniform_int_distribution<size_t>>();
        for (int i = 0; i < no_thr; i++) dists.push_back(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max()));

    };

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;
    virtual void run_thread(int id) = 0;

protected:
    size_t no_thr;
    Barrier *sync;
    vector<std::default_random_engine> prand;
    vector<std::uniform_int_distribution<size_t>> dists;

    inline size_t rand(int id) {
        return dists[id](prand[id]);
    }

    inline double randDouble(int id) {
        return double(rand(id)) / double(std::numeric_limits<size_t>::max());
    }

    /*
      Return random value from Cauchy distribution with mean "mu" and variance "gamma"
      http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Cauchy
      */
    inline double cauchy_g(int id, double mu, double gamma) {
        return mu + gamma * tan(M_PI * (randDouble(id) - 0.5));
    }

    /*
      Return random value from normal distribution with mean "mu" and variance "gamma"
      http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Gauss
      */
    inline double gauss(int id, double mu, double sigma){
        return mu + sigma * sqrt(-2.0 * log(randDouble(id))) * sin(2.0 * M_PI * randDouble(id));
    }
};

#endif
