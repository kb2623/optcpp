#ifndef _RDG_H_
#define _RDG_H_

#include "analize_algorithm.hpp"


class RDG: public AnalizeAlgorithm<double> {
public:
	RDG();
	~RDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>*) override;

protected:
	virtual vector<unsigned int> interact(double*, double, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);
	double calc_epsilon(BoundedObjectiveFunction<double>* func);

	size_t np;
	double alpha;
};

#endif
