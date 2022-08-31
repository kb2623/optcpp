#include "objective_function.hpp"

#include "common_funcs.hpp"
#include "thread_data.hpp"

// ------------------------ ObjectiveFunction ------------------------

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
BoundedObjectiveFunction<T>::BoundedObjectiveFunction() : ObjectiveFunction<T>(), RepairSolution<T>(), _x_bound_min(nullptr), _x_bound_max(nullptr) {}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(size_t dim) : ObjectiveFunction<T>(dim), RepairSolution<T>() {
	_x_bound_min = new double[dim];
	_x_bound_max = new double[dim];
}

template <typename T>
BoundedObjectiveFunction<T>::BoundedObjectiveFunction(const BoundedObjectiveFunction<T>& o) : ObjectiveFunction<T>(o), RepairSolution<T>(o) {
	_x_bound_min = copy_vector(o._x_bound_min, o._dim);
	_x_bound_max = copy_vector(o._x_bound_max, o._dim);
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

// ------------------------ ContiniousObjectiveFunction ------------------------

ContiniousObjectiveFunciton::ContiniousObjectiveFunciton() : BoundedObjectiveFunction() {}

ContiniousObjectiveFunciton::ContiniousObjectiveFunciton(size_t dim) : BoundedObjectiveFunction(dim) {}

ContiniousObjectiveFunciton::ContiniousObjectiveFunciton(const ContiniousObjectiveFunciton& o) : BoundedObjectiveFunction(o) {}

ContiniousObjectiveFunciton::~ContiniousObjectiveFunciton() {}

double* ContiniousObjectiveFunciton ::fix_max(double* x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_max[i];
	return x;
}

double* ContiniousObjectiveFunciton::fix_min(double* x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i];
	return x;
}

double* ContiniousObjectiveFunciton::fix_lim(double* x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i]) x[i] = _x_bound_max[i]; else if (x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i];
	return x;
}

double* ContiniousObjectiveFunciton::fix_mod(double* x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i] + dmod(x[i], _x_bound_max[i] - _x_bound_min[i]);
	return x;
}

double* ContiniousObjectiveFunciton::fix_rnd(double* x) {
	return x;
}

vector<double> ContiniousObjectiveFunciton ::fix_max(vector<double> x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_max[i];
	return x;
}

vector<double> ContiniousObjectiveFunciton::fix_min(vector<double> x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i];
	return x;
}

vector<double> ContiniousObjectiveFunciton::fix_lim(vector<double> x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i]) x[i] = _x_bound_max[i]; else if (x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i];
	return x;
}

vector<double> ContiniousObjectiveFunciton::fix_mod(vector<double> x) {
	for (size_t i = 0; i < this->_dim; i++)  if (x[i] >= _x_bound_max[i] || x[i] <= _x_bound_min[i]) x[i] = _x_bound_min[i] + dmod(x[i], _x_bound_max[i] - _x_bound_min[i]);
	return x;
}

vector<double> ContiniousObjectiveFunciton::fix_rnd(vector<double> x) {
	return x;
}

double ContiniousObjectiveFunciton::operator[](size_t i) {
	return _x_bound_min[i] + thread_td->randDouble() * (_x_bound_max[i] - _x_bound_min[i]);
}

double ContiniousObjectiveFunciton::dmod(double x, double y) {
	return x - int(x / y) * y;
}
