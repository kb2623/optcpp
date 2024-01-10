#ifndef _FII_H_
#define _FII_H_

#include "algorithm.hpp"


class FII: public AnalAlgorithm<double> {
public:
	FII();
	FII(const FII&);
	~FII();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;
};

#endif
