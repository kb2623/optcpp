#ifndef _REPAIR_SOLUTION_H_
#define _REPAIR_SOLUTION_H_

#include <cstddef>
#include <functional>
#include <vector>

using std::vector;

template<typename T>
class RepairSolution {
public:
	RepairSolution();
	~RepairSolution();

	/**
	 * @brief fix_max
	 * @param x
	 * @return
	 */
	virtual T* fix_max(T* x) = 0;
	/**
	 * @brief fix_min
	 * @param x
	 * @return
	 */
	virtual T* fix_min(T* x) = 0;
	/**
	 * @brief fix_lim
	 * @param x
	 * @return
	 */
	virtual T* fix_lim(T* x) = 0;
	/**
	 * @brief fix_mod
	 * @param x
	 * @return
	 */
	virtual T* fix_mod(T* x) = 0;
	/**
	 * @brief fix_rnd
	 * @param x
	 * @return
	 */
	virtual T* fix_rnd(T* x) = 0;
	/**
	 * @brief fix_max
	 * @param x
	 * @return
	 */
	virtual vector<T> fix_max(vector<T> x) = 0;
	/**
	 * @brief fix_min
	 * @param x
	 * @return
	 */
	virtual vector<T> fix_min(vector<T> x) = 0;
	/**
	 * @brief fix_lim
	 * @param x
	 * @return
	 */
	virtual vector<T> fix_lim(vector<T> x) = 0;
	/**
	 * @brief fix_mod
	 * @param x
	 * @return
	 */
	virtual vector<T> fix_mod(vector<T> x) = 0;
	/**
	 * @brief fix_rnd
	 * @param x
	 * @return
	 */
	virtual vector<T> fix_rnd(vector<T> x) = 0;
	/**
	 * @brief set_fix_solution_arr
	 * @param sol_fix
	 */
	void set_fix_solution_arr(std::function<T*(RepairSolution<T>&, T*)> sol_fix);
	/**
	 * @brief set_fix_solution_vec
	 * @param sol_fix
	 */
	void set_fix_solution_vec(std::function<vector<T>(RepairSolution<T>&, vector<T>)> sol_fix);

public:
	std::function<T*(RepairSolution<T>&, T*)> fix_solution_arr;
	std::function<vector<T>(RepairSolution<T>&, vector<T>)> fix_solution_vec;
};

#endif
