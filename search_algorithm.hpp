#ifndef _SEARCH_ALGORITHM_H_
#define _SEARCH_ALGORITHM_H_

#include "algorithm_parameters.hpp"
#include "thread_data.hpp"

#include <tuple>

#include <vector>
#include <string>
#include <stdexcept>

using std::string;
using std::vector;

// -------------------- SearchAlgorithm --------------------

using std::tuple;
using std::string;

template<typename T>
class SearchAlgorithm {
public:
	SearchAlgorithm();
	SearchAlgorithm(const SearchAlgorithm<T>&);
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
	 * @brief run_iteration
	 * @param tdata
	 * @param params
	 */
	virtual void run_iteration(thread_data& tdata, RunAlgParams<T>& params) = 0;
	/**
	 * @brief setParameters
	 * @param params
	 */
	virtual void setParameters(AlgParams& params);
	/**
	 * @brief run
	 * @param fit_fun
	 * @return
	 */
	virtual tuple<double, vector<T>> run(BoundedObjectiveFunction<T>& fit_fun);

protected:
	/**
	 * @brief initRun
	 * @param tdata
	 * @param func
	 * @return
	 */
	virtual RunAlgParams<T>& initRun(thread_data& tdata, BoundedObjectiveFunction<T>& func);

protected:
	/**
	 * @brief params Parameters for running the algorithm.
	 */
	AlgParams params;

};

// -------------------- ParallelSearchAlgorithm --------------------

template<typename T>
class ParallelSearchAlgorithm: public SearchAlgorithm<T>  {
public:
	ParallelSearchAlgorithm();
	ParallelSearchAlgorithm(size_t);
	ParallelSearchAlgorithm(size_t, size_t);
	ParallelSearchAlgorithm(const ParallelSearchAlgorithm<T>&);
	~ParallelSearchAlgorithm();

	/**
	 * @brief setParameters
	 * @param params
	 */
	virtual void setParameters(AlgParams& params) override;
	/**
	 * @brief run
	 * @param fitf
	 * @return
	 */
	virtual std::tuple<double, std::vector<T>> run(BoundedObjectiveFunction<T>& fitf) override;
	/**
	 * @brief initRun
	 * @param tdata
	 * @param func
	 * @return
	 */
	virtual RunParallelAlgParams<T>& initRun(thread_data& tdata, BoundedObjectiveFunction<T>& func) override;
	/**
	 * @brief run_thread
	 * @param tdata
	 * @param params
	 */
	void run_thread(thread_data& tdata, RunParallelAlgParams<T>& params);
	/**
	 * @brief run_iteration
	 * @param tdata
	 * @param params
	 */
	virtual void run_iteration(thread_data& tdata, RunAlgParams<T>& params) final;
	/**
	 * @brief run_iteration
	 * @param tdata
	 * @param params
	 */
	virtual void run_iteration(thread_data& tdata, RunParallelAlgParams<T>& params) = 0;

protected:
	/**
	 * @brief no_thr
	 */
	ulongint no_thr;
};

#endif
