#include "objective_function.hpp"

template <typename T>
ObjectiveFunction<T>::ObjectiveFunction() : RepairSolution<T>(), _dim(0), _no_fes(0) {}

template <typename T>
ObjectiveFunction<T>::ObjectiveFunction(const ObjectiveFunction<T>& o) : RepairSolution<T>(o), _dim(o.dim), _no_fes(o.no_fes) {}

template <typename T>
ObjectiveFunction<T>::ObjectiveFunction(size_t dim) : RepairSolution<T>(), _dim(dim), _no_fes(0) {}

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

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction() : ObjectiveFunction<T>(), _x_bound_min(nullptr), _x_bound_max(nullptr), max_num_evaluations(0) {}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(const BoundedObjectiveFunction<T>& o) : ObjectiveFunction<T>(o), max_num_evaluations(o.max_num_evaluations) {
	_x_bound_min = copy_vector(o.x_bound_min, o.dim);
	_x_bound_max = copy_vector(o.x_bound_max, o.dim);
}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(size_t dim, size_t g_max_num_evaluations) : ObjectiveFunction<T>(dim), max_num_evaluations(g_max_num_evaluations) {
	_x_bound_min = new double[dim];
	_x_bound_max = new double[dim];
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
	_x_bound_min = copy_vector(low_lim, _x_bound_min, this->dim);
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
	_x_bound_max = copy_vector(up_lim, _x_bound_max, this->dim);
}
