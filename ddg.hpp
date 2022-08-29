#ifndef _DDG_H_
#define _DDG_H_

#include "analize_algorithm.hpp"
#include "objective_function.hpp"

#include <algorithm>

class DDG: public AnalizeAlgorithm<double> {
public:
	DDG();
	~DDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams* params) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>* fit_fun) override;

protected:
	double get_epsilon_addi(double, double, double, double);

	double epsilon_multi;
	double epsilon_addi;
};

#endif
