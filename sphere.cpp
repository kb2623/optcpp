#include "sphere.hpp"

double Sphere::calc (double *x) {
	double s = 0;
	for (int i = 0; i < dim; i++) s += x[i] * x[i];
	return s;
}

void Sphere::test_func(double *x, double *f, int mx) {
	for (int i = 0; i < mx; i++) f[i] = calc(&x[i * dim]);
}
