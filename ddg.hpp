#ifndef _DDG_H_
#define _DDG_H_

#include "algorithm.hpp"

#include <algorithm>

class DDG: public AnalAlgorithm<double> {
public:
	DDG();
	DDG(const DDG&);
	~DDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	double get_epsilon_addi(double, double, double, double, BoundedObjectiveFunction<double>&);

	double epsilon_multi;
	double epsilon_addi;
};

#endif
