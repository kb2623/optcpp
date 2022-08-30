#ifndef _SEARCH_ALGORITHM_H_
#define _SEARCH_ALGORITHM_H_

#include "objective_function.hpp"
#include "algorithm_parameters.hpp"
#include "stopping_condition.hpp"

#include <cmath>
#include <tuple>
#include <string>
#include <atomic>
#include <mutex>
#include <random>

using std::tuple;
using std::string;

template<typename T>
class SearchAlgorithm : public StoppingCondition {
public:
	SearchAlgorithm();
	~SearchAlgorithm();

	/**
	 * @brief info
	 * @return
	 */
	virtual string info() = 0;
	/**
	 * @brief sinfo
	 * @return
	 */
	virtual string sinfo() = 0;
	/**
	 * @brief run_iteration Execute one iteration of an search algorithm.
	 */
	virtual void run_iteration() = 0;
	/**
	 * @brief setParameters
	 * @param params
	 */
	virtual void setParameters(AlgParams* params);
	/**
	 * @brief run
	 * @param fit_fun
	 * @return
	 */
	virtual tuple<double, vector<T>> run(BoundedObjectiveFunction<T>* fit_fun);
	/**
	 * @brief no_gen
	 * @return
	 */
	inline unsigned long long int no_gen() const;

public:
	/**
	 * @brief x_best Best solution's components value.
	 */
	vector<T> x_best;
	/**
	 * @brief f_best Best solution's fitness value.
	 */
	double f_best;

protected:
	virtual void initRun(BoundedObjectiveFunction<T>* func);
	/**
	 * @brief makeNewArrayIndividual
	 * @return
	 */
	T* makeNewArrayIndividual();
	/**
	 * @brief makeNewVectorIndividual
	 * @return
	 */
	vector<T> makeNewVectorIndividual();
	/**
	 * @brief setBestSolution
	 * @param x
	 * @param fitness
	 */
	void setBestSolution(T* x, double fitness);
	/**
	 * @brief rand Generate a pseudo-random integral number in [0, RAND_MAX].
	 * @return Pseudo-random intergral value.
	 */
	virtual size_t rand();
	/**
	 * @brief randDouble Generate a pseudo-random floating point number in [0, 1]
	 * @return Pseudo-random floating point value.
	 */
	double randDouble();

protected:
	virtual bool max_no_fes() override;
	virtual bool max_no_gen() override;
	virtual bool target_value() override;

protected:
	/**
	 * @brief no_gen
	 */
	std::atomic<unsigned long long int> _no_gen;
	/**
	 * @brief params Parameters for running the algorithm.
	 */
	AlgParams params;
	/**
	 * @brief fitf
	 */
	BoundedObjectiveFunction<T>& fitf;
	/**
	 * @brief stop_cond
	 */
	std::function<bool(SearchAlgorithm&)> stop_cond;
	/**
	 * @brief prand List of random egines.
	 */
	vector<std::default_random_engine> prand;
	/**
	 * @brief dists List of full number uniform random distribution.
	 */
	vector<std::uniform_int_distribution<size_t>> dists;
	/**
	 * @brief best_lock Lock for updating global best.
	 */
	std::mutex best_lock;

};

#endif
