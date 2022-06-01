#ifndef _RDGV2_H_
#define _RDGV2_H_

#include "rdg.hpp"

class RDGv2: public RDG {
public:
	double eps = 2.2204e-16;

	RDGv2();
	~RDGv2();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
	vector<unsigned int> interact(double*, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);
	double gamma(double);
};

#endif
