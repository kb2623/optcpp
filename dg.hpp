#ifndef _DG_H_
#define _DG_H_

#include "analize_algorithm.hpp"

class DG: public AnalizeAlgorithm<double> {
public:
	DG();
	~DG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>*) override;

protected:
	double epsilon(double, double, double, double);
};

#endif
