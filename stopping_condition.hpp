#ifndef _STOPPING_CONDITION_H_
#define _STOPPING_CONDITION_H_

#include <chrono>
#include <limits>
#include <functional>

class StoppingCondition {
public:
	StoppingCondition();
	~StoppingCondition();

public:
	/**
	 * @brief max_no_fes
	 * @return
	 */
	virtual bool max_no_fes() = 0;
	/**
	 * @brief max_no_gen
	 * @return
	 */
	virtual bool max_no_gen() = 0;
	/**
	 * @brief target_value
	 * @return
	 */
	virtual bool target_value() = 0;
	/**
	 * @brief run_time
	 * @return
	 */
	bool run_time();
	/**
	 * @brief no_limit
	 * @return
	 */
	bool no_limit();
	/**
	 * @brief set_end_run
	 * @param condition
	 */
	void set_end_run(std::function<bool(StoppingCondition&)> condition);
	/**
	 * @brief start_timer
	 */
	void start_timer();
	/**
	 * @brief set_max_no_fes
	 * @param lim_fes
	 */
	void set_max_no_fes(unsigned long long int lim_fes);
	/**
	 * @brief set_max_no_gen
	 * @param lim_get
	 */
	void set_max_no_gen(unsigned long long int lim_get);
	/**
	 * @brief set_fitness_target_value
	 * @param fitness
	 */
	void set_fitness_target_value(double fitness);

protected:
	/**
	 * @brief end_run Link to a stopping cretiria.
	 */
	std::function<bool(StoppingCondition&)> end_run;
	/**
	 * @brief max_execution_time Maximum execution time of the algorithm.
	 */
	double max_execution_time;
	/**
	 * @brief start_time Start time of the algorithm.
	 */
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	/**
	 * @brief value_target
	 */
	double fitness_target_value = std::numeric_limits<double>::infinity();
	/**
	 * @brief lim_no_fes
	 */
	unsigned long long int lim_no_fes;
	/**
	 * @brief lim_no_gen
	 */
	unsigned long long int lim_no_gen;

};

#endif
