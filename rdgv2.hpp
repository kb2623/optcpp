#ifndef _RDGV2_H_
#define _RDGV2_H_

#include "rdg.hpp"

class RDGv2: public RDG {
public:
	double eps = 2.2204e-16;

	RDGv2();
	RDGv2(const RDGv2&);
	~RDGv2();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	vector<size_t> interact(double*, double, vector<size_t>, vector<size_t>, vector<size_t>&, BoundedObjectiveFunction<double>&);
	double gamma(double);
};

#endif
