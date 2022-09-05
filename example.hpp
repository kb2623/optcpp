#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "search_algorithm.hpp"

// -------------------- BarParams --------------------

template <typename T>
class BarParams : public RunParallelAlgParams<T> {
public:
	BarParams();
	~BarParams();

public:
	vector<double> xf;
	vector<vector<double>> x;
};

// -------------------- Bar --------------------

class Bar : public ParallelSearchAlgorithm<double> {
public:
	Bar();
	Bar(size_t);
	Bar(size_t, size_t);
	~Bar();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams&) override;
	virtual RunParallelAlgParams<double>& initRun(BoundedObjectiveFunction<double>&) override;
	virtual tuple<double, vector<double>> run(BoundedObjectiveFunction<double>&) override;
	virtual void run_iteration(thread_data&, RunParallelAlgParams<double>&) override;

protected:
	size_t np;
};

#endif
