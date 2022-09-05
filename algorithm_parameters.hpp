#ifndef _ALGORITHM_PARAMETERS_H_
#define _ALGORITHM_PARAMETERS_H_

#include "objective_function.hpp"
#include "stopping_condition.hpp"
#include "barrier.hpp"

#include <map>
#include <any>
#include <tuple>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

using std::any;
using std::map;
using std::tuple;
using std::string;
using std::vector;

// -------------------- AlgParams --------------------

class AlgParams {
public:
	AlgParams();
	~AlgParams();

	/**
	 * @brief getParamsVals
	 * @return
	 */
	map<string, any>& getParamsVals();
	/**
	 * @brief setParamsVals
	 * @param params
	 */
	void setParamsVals(map<string, any> params);
	/**
	 * @brief setParamVal
	 * @param key
	 * @param value
	 */
	void setParamVal(string key, any value);
	/**
	 * @brief setParamVal
	 * @param key
	 * @param pvalue
	 */
	void setParamVal(string key, any* pvalue);
	/**
	 * @brief has
	 * @param key
	 * @return
	 */
	bool has(string key);
	/**
	 * @brief operator []
	 * @param key
	 * @return
	 */
	any& operator[](string key);
	/**
	 * @brief operator ()
	 * @param key
	 * @return
	 */
	any operator()(string key);
	/**
	 * @brief operator []
	 * @param key
	 * @return
	 */
	template <typename T> T& operator[](string key);
	/**
	 * @brief operator ()
	 * @param key
	 * @return
	 */
	template <typename T> T operator()(string key);
	/**
	 * @brief at
	 * @param key
	 * @return
	 */
	template <typename T> T at(string key);

protected:
	/**
	 * @brief params
	 */
	map<string, any> params;
};

/**
 * @brief getParam
 * @param params
 * @param key
 * @param dval
 * @return
 */
template <typename T>
T getParam(AlgParams& params, string key, T dval);

/**
 * @brief getParam
 * @param params
 * @param key
 * @param dval
 * @return
 */
template <typename T>
T getParam(AlgParams* params, string key, T dval);

// -------------------- RunAlgParams --------------------

typedef unsigned long long int ulongint;

template <typename T>
class RunAlgParams : public StoppingCondition {
public:
	RunAlgParams();
	RunAlgParams(BoundedObjectiveFunction<T>& fitf);
	~RunAlgParams();

	/**
	 * @brief operator ++
	 * @return
	 */
	ulongint operator++();
	/**
	 * @brief setBestSolution
	 * @param x
	 * @param f
	 */
	virtual void setBestSolution(T* x, double f);
	/**
	 * @brief setBestSolution
	 * @param x
	 * @param f
	 */
	virtual void setBestSolution(vector<T> x, double f);
	/**
	 * @brief getBestSolution
	 * @return
	 */
	tuple<double, vector<T>> getBestSolution();
	/**
	 * @brief makeNewArrayIndividual
	 * @param tdata
	 * @return
	 */
	T* makeNewArrayIndividual(thread_data& tdata);
	/**
	 * @brief makeNewVectorIndividual
	 * @param tdata
	 * @return
	 */
	vector<T> makeNewVectorIndividual(thread_data& tdata);
	/**
	 * @brief fitf
	 * @return
	 */
	BoundedObjectiveFunction<T>& fitf() const;

public:
	virtual bool max_no_fes() override;
	virtual bool max_no_gen() override;
	virtual bool target_value() override;

protected:
	/**
	 * @brief _no_gen
	 */
	std::atomic<ulongint> _no_gen;
	/**
	 * @brief fitf
	 */
	BoundedObjectiveFunction<T>& _fitf;
	/**
	 * @brief x_best Best solution's components value.
	 */
	vector<T> x_best;
	/**
	 * @brief f_best Best solution's fitness value.
	 */
	double f_best;

};

// -------------------- RunParallelAlgParams --------------------

template <typename T>
class RunParallelAlgParams : public RunAlgParams<T> {
public:
	RunParallelAlgParams();
	RunParallelAlgParams(Barrier& b);
	~RunParallelAlgParams();

	virtual void setBestSolution(T* x, double f) override;
	void arrive_and_wait();

protected:
	/**
	 * @brief best_lock Lock for updating global best.
	 */
	std::mutex best_lock;
	/**
	 * @brief sync
	 */
	Barrier& sync;
};

#endif
