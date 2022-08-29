#ifndef _XDG_H_
#define _XDG_H_

#include "analize_algorithm.hpp"
#include "objective_function.hpp"

#include <algorithm>

class XDG: public AnalizeAlgorithm<double> {
public:
	XDG();
	~XDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams* params) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(BoundedObjectiveFunction<double>* x) override;

protected:
	double epsilon(double, double, double, double);

	double _epsilon;
};

#endif
