#ifndef _RDGV3_H_
#define _RDGV3_H_

#include "rdgv2.hpp"

class RDGv3: public RDGv2 {
public:
	RDGv3();
	RDGv3(const RDGv3&);
	~RDGv3();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	vector<size_t> interact(double*, double, vector<size_t>, vector<size_t>, vector<size_t>&, BoundedObjectiveFunction<double>&);

	double tnf = 0.1;
};

#endif
