#ifndef _PARALLEL_SEARCH_ALGORITHM_
#define _PARALLEL_SEARCH_ALGORITHM_

#include "search_algorithm.hpp"

#include <random>
#include <thread>
#include "barrier.hpp"

class ParallelSearchAlgorithm: public SearchAlgorithm {
public:
	ParallelSearchAlgorithm();
	ParallelSearchAlgorithm(size_t);
	ParallelSearchAlgorithm(size_t, size_t);

	virtual void run_iteration(int) = 0;
	virtual void run_thread(int);
	virtual void fix_solution(double*, int);
	virtual std::tuple<double, std::vector<double>> run(TestFuncBounds *) override;

	virtual void run_iteration() final;
	virtual void fix_solution(double*) final;

protected:
	size_t no_thr;
	Barrier *sync;
	vector<std::default_random_engine> prand;
	vector<std::uniform_int_distribution<size_t>> dists;

	/**
	  * @brief rand Get random generator for a thread
	  * @param id   ID of a thread
	  * @return     Random geneator
	  */
	size_t rand(int);
	/**
	  * @brief randDouble Return random value with uniform distribution [0, 1)
	  * @return [0, 1)
	  */
	double randDouble(int);
	/**
	  * @brief cauchy_g Return random value from Cauchy distribution with mean "mu" and variance "gamma"
	  *                 http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Cauchy
	  * @param id       Id of a thread
	  * @param mu       Parameter mu
	  * @param gamma    Parameter sigma
	  * @return
	  */
	double cauchy_g(int, double, double);
	/**
	  * @brief gauss Return random value from normal distribution with mean "mu" and variance "gamma"
	  *              http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Gauss
	  * @param id    Id of a thread
	  * @param mu    Parameter mu
	  * @param sigma Parameter sigma
	  * @return
	  */
	double gauss(int, double, double);
};

#endif
