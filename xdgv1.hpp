#ifndef _XDGV1_H_
#define _XDGV1_H_

#include "xdg.hpp"
#include "objective_function.hpp"

#include <algorithm>

class XDGv1: public XDG {
public:
	XDGv1();
	~XDGv1();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>*) override;

protected:
	double epsilon(double, double, double, double);
	double calc_epsilon();

	double alpha = 10e-12;
	unsigned int np = 50;
	double _epsilon;
};

#endif
