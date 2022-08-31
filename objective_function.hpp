#ifndef _OBJECTIVE_FUNCTION_H_
#define _OBJECTIVE_FUNCTION_H_

#include <cstddef>
#include <atomic>

#include "repair_solution.hpp"

// ------------------------ ObjectiveFunction ------------------------

template <typename T>
class ObjectiveFunction {
public:
	ObjectiveFunction();
	ObjectiveFunction(size_t);
	ObjectiveFunction(const ObjectiveFunction<T>&);
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
	unsigned long long int no_fes() const;
	/**
	 * @brief dim
	 * @return
	 */
	size_t dim() const;
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

// ------------------------ BoundedObjectiveFunction ------------------------

template <typename T>
class BoundedObjectiveFunction: public ObjectiveFunction<T>, public RepairSolution<T> {
public:
	BoundedObjectiveFunction();
	BoundedObjectiveFunction(size_t);
	BoundedObjectiveFunction(const BoundedObjectiveFunction<T>&);
	~BoundedObjectiveFunction();

	/**
	 * @brief x_bound_min
	 * @return
	 */
	T* x_bound_min() const;
	/**
	 * @brief x_bound_min
	 * @param index
	 * @return
	 */
	T x_bound_min(size_t index) const;
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
	T* x_bound_max() const;
	/**
	 * @brief x_bound_max
	 * @param index
	 * @return
	 */
	T x_bound_max(size_t index) const;
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
};

// ------------------------ ContiniousObjectiveFunction ------------------------

class ContiniousObjectiveFunciton : public BoundedObjectiveFunction<double> {
public:
	ContiniousObjectiveFunciton();
	ContiniousObjectiveFunciton(size_t);
	ContiniousObjectiveFunciton(const ContiniousObjectiveFunciton&);
	~ContiniousObjectiveFunciton();

	virtual double* fix_max(double* x) override;
	virtual double* fix_min(double* x) override;
	virtual double* fix_lim(double* x) override;
	virtual double* fix_mod(double* x) override;
	virtual double* fix_rnd(double* x) override;
	virtual vector<double> fix_max(vector<double> x) override;
	virtual vector<double> fix_min(vector<double> x) override;
	virtual vector<double> fix_lim(vector<double> x) override;
	virtual vector<double> fix_mod(vector<double> x) override;
	virtual vector<double> fix_rnd(vector<double> x) override;
	virtual double operator[](size_t index) override;

private:
	double dmod(double x, double y);

};

#endif
