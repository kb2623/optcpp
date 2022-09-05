#ifndef _ANALIZE_ALGORITHM_H_
#define _ANALIZE_ALGORITHM_H_

#include "objective_function.hpp"
#include "algorithm_parameters.hpp"

#include <limits>
#include <atomic>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
#include <random>

using std::string;
using std::tuple;
using std::vector;

template <typename T>
class AnalizeAlgorithm {
public:
	AnalizeAlgorithm();
	AnalizeAlgorithm(const AnalizeAlgorithm&);
	~AnalizeAlgorithm();

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
	 * @brief setParameters
	 */
	virtual void setParameters(AlgParams& params) = 0;
	/**
	 * @brief run
	 * @param func
	 * @return
	 */
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<T>& func) = 0;

protected:
	/**
	 * @brief initRun
	 * @param func
	 */
	virtual RunAlgParams<T>& initRun(thread_data& tdata, BoundedObjectiveFunction<T>& func);
	/**
	 * @brief run_iteration
	 * @param tdata
	 * @param params
	 */
	virtual void run_iteration(thread_data& tdata, RunAlgParams<T>& params);

};

#endif
