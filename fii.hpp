#ifndef _FII_H_
#define _FII_H_

#include "analize_algorithm.hpp"


class FII: public AnalizeAlgorithm<double> {
public:
	FII();
	~FII();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>*) override;
};

#endif
