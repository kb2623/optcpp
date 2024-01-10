#ifndef _DDGV1_H_
#define _DDGV1_H_

#include "ddg.hpp"

class DDGv1: public DDG {
public:
	DDGv1();
	DDGv1(const DDGv1&);
	~DDGv1();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	double get_epsilon_addi(double, double, double, double, BoundedObjectiveFunction<double>&);
	double calc_epsilon(BoundedObjectiveFunction<double>&);

	double alpha;
	unsigned int np = 50;
	double epsilon_addi = 0;
};

#endif
