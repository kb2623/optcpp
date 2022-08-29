#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "parallel_search_algorithm.hpp"

class Bar : public ParallelSearchAlgorithm<double> {
public:
	Bar();
	Bar(size_t);
	Bar(size_t, size_t);
	~Bar();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	void initRun(BoundedObjectiveFunction<double>*) override;
	virtual tuple<double, vector<double>> run(BoundedObjectiveFunction<double>*) override;
	virtual void run_iteration() override;

protected:
	size_t np;
	vector<double> xf;
	vector<vector<double>> x;
};

#endif
