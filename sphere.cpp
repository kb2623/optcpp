#include "sphere.hpp"

Sphere::Sphere() : ContiniousObjectiveFunciton() { }

Sphere::Sphere(size_t dim, double min_lim, double max_lim) : ContiniousObjectiveFunciton(dim) {
	for (int i = 0; i < dim; i++) _x_bound_min[i] = min_lim, _x_bound_max[i] = max_lim;
}

Sphere::~Sphere() { }

double Sphere::calc(double *x) {
	double s = 0;
	for (int i = 0; i < _dim; i++) s += x[i] * x[i];
	return s;
}

double Sphere::func(double *x, int dim) {
	return calc(x);
}
