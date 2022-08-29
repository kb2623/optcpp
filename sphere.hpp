#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "objective_function.hpp"

#include <vector>

class Sphere: public BoundedObjectiveFunction<double> {
public:
	Sphere() = default;
	Sphere(size_t dim, size_t evals, double min_lim = -100, double max_lim = 100) : BoundedObjectiveFunction(dim, evals) {
		for (int i = 0; i < dim; i++) _x_bound_min[i] = min_lim, _x_bound_max[i] = max_lim;
	};
	~Sphere() = default;

	virtual double func (double* x, int dim) override;
	virtual double operator[] (size_t i) override;

private:
	double calc (double*);
};

#endif
