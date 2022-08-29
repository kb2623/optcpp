#include "sphere.hpp"

double Sphere::calc (double *x) {
	double s = 0;
	for (int i = 0; i < _dim; i++) s += x[i] * x[i];
	return s;
}

double Sphere::func(double *x, int dim) {
	return calc(x);
}

double Sphere::operator[](size_t i) {
	return 23.34; // FIXME add some sort of random engine
}
