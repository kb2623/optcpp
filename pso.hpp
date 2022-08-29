#ifndef _PSO_H_
#define _PSO_H_

#include "continuous_parallel_search_algorithm.hpp"
#include "cc_optimizer.hpp"

using namespace std;

class PSO: public ContinuousParallelSearchAlgorithm, public CooperativeCoevolutionOptimizer {
public:
	PSO();
	PSO(size_t);
	PSO(size_t, size_t);
	~PSO();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	void initRun(TestFuncBounds<double>*) override;
	virtual tuple<double, vector<double>> run(TestFuncBounds<double>*) override;
	virtual void run_iteration() override;

protected:
	vector<double*> pop;
	vector<double> popf;

	vector<double*> lpop;
	vector<double> lpopf;

	vector<double*> V;
	vector<double> vMin;
	vector<double> vMax;

	size_t np;
	double C1;
	double C2;
	vector<double> W;
};

#endif
