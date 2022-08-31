#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "objective_function.hpp"

#include <vector>

class Sphere: public ContiniousObjectiveFunciton {
public:
	Sphere();
	Sphere(size_t dim, double min_lim = -100, double max_lim = 100);
	~Sphere();

	virtual double func (double* x, int dim) override;
	virtual double operator[] (size_t i) override;

private:
	/**
	 * @brief calc
	 * @param x
	 * @return
	 */
	double calc (double* x);

};

#endif
