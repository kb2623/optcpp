#ifndef _SEARCH_ALGORITHM_H_
#define _SEARCH_ALGORITHM_H_

#include "testfunc.hpp"
#include "algorithm_parameters.hpp"

#include <limits>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
#include <atomic>
#include <mutex>
#include <functional>

using std::tuple;
using std::vector;
using std::string;

class SearchAlgorithm {
public:
	SearchAlgorithm();
	~SearchAlgorithm();

	virtual string info() = 0;
	virtual string sinfo() = 0;
	virtual void run_iteration() = 0;

	virtual void fix_solution(double*);
	virtual void setParameters(AlgParams*);
	virtual tuple<double, vector<double>> run(TestFuncBounds*);

	double eval(double*);
	size_t get_nfes();
	bool nfes_stop_cond();
	bool setParameters();

	vector<double> x_best;
	double  f_best;
	std::mutex best_lock;

protected:
	virtual void initRun(TestFuncBounds* func);
	double* makeNewIndividual();
	void setBestSolution(double*, double);

	/**
	  * @brief randDouble Return random value with uniform distribution [0, 1)
	  * @return [0, 1)
	  */
	double randDouble();
	/**
	  * http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Cauchy
	  * @brief cauchy_g Return random value from Cauchy distribution with mean "mu" and variance "gamma"
	  * @return Real number
	  */
	double cauchy_g(double, double);
	/**
	  * http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Gauss
	  * @brief gauss Return random value from normal distribution with mean "mu" and variance "gamma"
	  * @return Real number
	  */
	double gauss(double, double);

protected:
	std::atomic<unsigned int> nfes;
	std::function<bool(SearchAlgorithm&)> stop_cond;
	TestFuncBounds* func;
	AlgParams params;
};

inline void copy_vector(double*, double*, size_t);

class SolutionFix {
public:
	SolutionFix();
	~SolutionFix();

	virtual void to_upper(double*, size_t);
	virtual void to_lower(double*, size_t);
	virtual void to_randn(double*, size_t);
	virtual void to_modus(double*, size_t);
};

#endif
