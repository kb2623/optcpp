#ifndef _DG_H_
#define _DG_H_

#include "algorithm.hpp"

class DG: public AnalAlgorithm<double> {
public:
	DG();
	DG(const DG&);
	~DG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	double epsilon(double, double, double, double);
};

#endif
