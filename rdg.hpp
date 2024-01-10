#ifndef _RDG_H_
#define _RDG_H_

#include "algorithm.hpp"


class RDG: public AnalAlgorithm<double> {
public:
	RDG();
	RDG(const RDG&);
	~RDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	virtual vector<size_t> interact(double*, double, double, vector<size_t>, vector<size_t>, vector<size_t>&, BoundedObjectiveFunction<double>&);
	double calc_epsilon(BoundedObjectiveFunction<double>&);

	size_t np;
	double alpha;
};

#endif
