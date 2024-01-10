#ifndef _XDGV1_H_
#define _XDGV1_H_

#include "xdg.hpp"
#include "objective_function.hpp"

#include <algorithm>

class XDGv1: public XDG {
public:
	XDGv1();
	XDGv1(const XDGv1&);
	~XDGv1();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	double epsilon(double, double, double, double, size_t) override;
	double calc_epsilon(BoundedObjectiveFunction<double>&);

	double alpha = 10e-12;
	unsigned int np = 50;
	double _epsilon;
};

#endif
