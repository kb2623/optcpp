#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "objective_function.hpp"

class Sphere: public BoundedObjectiveFunction<double> {
public:
	Sphere();
	Sphere(size_t dim, double min_lim = -100, double max_lim = 100);
	Sphere(const Sphere&);
	~Sphere();

	/**
	 */
	virtual double func(double*, size_t) override;

private:
	/**
	 */
	double* _x_bound_min;
	/**
	 */
	double* _x_bound_max;

};

#endif
