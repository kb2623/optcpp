#ifndef _DG_H_
#define _DG_H_

#include "analize_algorithm.hpp"

class DG: public AnalizeAlgorithm {
public:
	DG();
	~DG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
	double epsilon(double, double, double, double);
};

#endif
