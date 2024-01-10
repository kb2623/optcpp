#include "sphere.hpp"

Sphere::Sphere() : BoundedObjectiveFunction<double>() {}

Sphere::Sphere(const Sphere& o) : BoundedObjectiveFunction(o) {
	_x_bound_min = new double[_dim];
	_x_bound_max = new double[_dim];
	for (int i = 0; i < _dim; i++) {
		_x_bound_min[i] = o._x_bound_min[i];
		_x_bound_max[i] = o._x_bound_max[i];
	}
}

Sphere::Sphere(size_t dim, double min_lim, double max_lim) : BoundedObjectiveFunction<double>(dim) {
	_x_bound_min = new double[dim];
	_x_bound_max = new double[dim];
	for (int i = 0; i < dim; i++) _x_bound_min[i] = min_lim, _x_bound_max[i] = max_lim;
}

Sphere::~Sphere() { }

double Sphere::func(double *x, size_t d) {
	double s = 0;
	for (int i = 0; i < d; i++) s += x[i] * x[i];
	return s;
}

