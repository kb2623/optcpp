#ifndef _CC_OPTIMIZER_H_
#define _CC_OPTIMIZER_H_

#include <vector>
#include <cstddef>

using std::vector;

class CooperativeCoevolutionOptimizer {
public:
	CooperativeCoevolutionOptimizer();
	~CooperativeCoevolutionOptimizer();

	virtual void run_iteration() = 0;
	virtual vector<size_t> getActiveComponents() = 0;
};

#endif
