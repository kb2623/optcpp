#ifndef _TESTFUNC_H_
#define _TESTFUNC_H_

#include <cstddef>
#include <atomic>

#include "repair_solution.hpp"

template <typename T>
class ObjectiveFunction : public RepairSolution<T> {
public:
	ObjectiveFunction();
	ObjectiveFunction(const ObjectiveFunction<T>&);
	ObjectiveFunction(size_t);
	~ObjectiveFunction();

	/**
	 * @brief func
	 * @param x
	 * @param dim
	 */
	virtual double func(T* x, int dim) = 0;
	/**
	 * @brief operator []
	 * @param index
	 * @return
	 */
	virtual T operator[](size_t index) = 0;
	/**
	 * @brief no_fes
	 * @return
	 */
	inline unsigned long long int no_fes() const;
	/**
	 * @brief dim
	 * @return
	 */
	inline size_t dim() const;
	/**
	 * @brief eval
	 * @param x
	 * @return
	 */
	double eval(T* x);
	/**
	 * @brief operator ()
	 * @param x
	 * @return
	 */
	double operator()(T* x);
	/**
	 * @brief eval
	 * @param x
	 * @return
	 */
	double eval(vector<T> x);
	/**
	 * @brief operator ()
	 * @param x
	 * @return
	 */
	double operator()(vector<T> x);
	/**
	 * @brief reset
	 */
	void reset();

protected:
	/**
	 * @brief dim Size of the problem.
	 */
	size_t _dim;
	/**
	 * @brief no_fes Number of function evaluations.
	 */
	std::atomic<unsigned long long int> _no_fes;
};

template <typename T>
class BoundedObjectiveFunction: public ObjectiveFunction<T> {
public:
	BoundedObjectiveFunction();
	BoundedObjectiveFunction(const BoundedObjectiveFunction<T>&);
	BoundedObjectiveFunction(size_t);
	BoundedObjectiveFunction(size_t, size_t);
	~BoundedObjectiveFunction();

	/**
	 * @brief x_bound_min
	 * @return
	 */
	inline T* x_bound_min() const;
	/**
	 * @brief x_bound_min
	 * @param index
	 * @return
	 */
	inline T x_bound_min(size_t index) const;
	/**
	 * @brief x_bound_min
	 * @param low_lim
	 * @return
	 */
	void x_bound_min(T* low_lim);
	/**
	 * @brief x_bound_max
	 * @return
	 */
	inline T* x_bound_max() const;
	/**
	 * @brief x_bound_max
	 * @param index
	 * @return
	 */
	inline T x_bound_max(size_t index) const;
	/**
	 * @brief x_bound_max
	 * @param up_lim
	 * @return
	 */
	void x_bound_max(T* up_lim);

protected:
	/**
	 * @brief _x_bound_min Lower limit of the problem search space.
	 */
	T* _x_bound_min;
	/**
	 * @brief _x_bound_max Upper limit of the problem search space.
	 */
	T* _x_bound_max;
	/**
	 * @brief max_num_evaluations Maximum number of evaluations.
	 */
	size_t max_num_evaluations;
};

#endif
