#ifndef _OBJECTIVE_FUNCTION_H_
#define _OBJECTIVE_FUNCTION_H_

#include "common_funcs.hpp"

#include <cstddef>
#include <atomic>
#include <vector>

using std::vector;

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
	virtual double func(T*, size_t) = 0;
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
	double eval(T*);
	/**
	 * @brief operator ()
	 * @param x
	 * @return
	 */
	double operator()(T*);
	/**
	 * @brief eval
	 * @param x
	 * @return
	 */
	double eval(vector<T>);
	/**
	 * @brief operator ()
	 * @param x
	 * @return
	 */
	double operator()(vector<T>);
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
ObjectiveFunction<T>::ObjectiveFunction() : _dim(0), _no_fes(0) {}

template <typename T>
ObjectiveFunction<T>::ObjectiveFunction(const ObjectiveFunction<T>& o) : _dim(o._dim), _no_fes(o._no_fes.load()) {}

template <typename T>
ObjectiveFunction<T>::ObjectiveFunction(size_t dim) : _dim(dim), _no_fes(0) {}

template <typename T>
ObjectiveFunction<T>::~ObjectiveFunction() {}

template <typename T>
inline unsigned long long int ObjectiveFunction<T>::no_fes() const {
	return _no_fes;
}

template <typename T>
inline size_t ObjectiveFunction<T>::dim() const {
	return _dim;
}

template <typename T>
double ObjectiveFunction<T>::eval(T* x) {
	_no_fes++;
	return func(x, _dim);
}

template <typename T>
double ObjectiveFunction<T>::eval(vector<T> x) {
	return eval(&x[0]);
}

template <typename T>
double ObjectiveFunction<T>::operator()(T* x) {
	return eval(x);
}

template <typename T>
double ObjectiveFunction<T>::operator()(vector<T> x) {
	return eval(x);
}

template <typename T>
void ObjectiveFunction<T>::reset() {
	_no_fes = 0;
}

// ------------------------ BoundedObjectiveFunction ------------------------

template <typename T>
class BoundedObjectiveFunction: public ObjectiveFunction<T> {
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

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction() : ObjectiveFunction<T>(), _x_bound_min(nullptr), _x_bound_max(nullptr) {}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(size_t dim) : ObjectiveFunction<T>(dim) {
	_x_bound_min = new double[dim];
	_x_bound_max = new double[dim];
}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(const BoundedObjectiveFunction<T>& o) : ObjectiveFunction<T>(o) {
	_x_bound_min = copy_vector<T>(o._x_bound_min, o._dim);
	_x_bound_max = copy_vector<T>(o._x_bound_max, o._dim);
}

template <typename T>
BoundedObjectiveFunction<T>::~BoundedObjectiveFunction() {
	if (_x_bound_max != nullptr) {
		delete [] _x_bound_max;
		_x_bound_max = nullptr;
	}
	if (_x_bound_min != nullptr) {
		delete [] _x_bound_min;
		_x_bound_min = nullptr;
	}
}

template <typename T>
inline T* BoundedObjectiveFunction<T>::x_bound_min() const {
	return _x_bound_min;
}

template <typename T>
inline T BoundedObjectiveFunction<T>::x_bound_min(size_t index) const {
	if (index < this->dim()) return _x_bound_min[index];
	else return _x_bound_min[0];
}

template <typename T>
void BoundedObjectiveFunction<T>::x_bound_min(T* low_lim) {
	_x_bound_min = copy_vector<T>(low_lim, _x_bound_min, this->dim);
}

template <typename T>
inline T* BoundedObjectiveFunction<T>::x_bound_max() const {
	return _x_bound_max;
}

template <typename T>
inline T BoundedObjectiveFunction<T>::x_bound_max(size_t index) const {
	if (index < this->dim()) return _x_bound_max[index];
	return _x_bound_max[0];
}

template <typename T>
void BoundedObjectiveFunction<T>::x_bound_max(T* up_lim) {
	_x_bound_max = copy_vector<T>(up_lim, _x_bound_max, this->dim);
}

#endif
