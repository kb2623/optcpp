#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "testfunc.hpp"

#include <vector>

class Sphere: public TestFuncBounds {
public:
	Sphere(size_t dim, size_t evals, double min_lim = -100, double max_lim = 100) : TestFuncBounds(dim, evals) {
		for (int i = 0; i < dim; i++) x_bound_min[i] = min_lim, x_bound_max[i] = max_lim;
	};
	~Sphere() {};

	virtual void test_func (double*, double*, int) override;

private:
	double calc (double*);
};

#endif
