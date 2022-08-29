#ifndef _RDGV3_H_
#define _RDGV3_H_

#include "rdgv2.hpp"

class RDGv3: public RDGv2 {
public:
	RDGv3();
	~RDGv3();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>*) override;

protected:
	vector<unsigned int> interact(double*, double, vector<unsigned int>, vector<unsigned int>, vector<unsigned int>&);

	double tnf = 0.1;
};

#endif
