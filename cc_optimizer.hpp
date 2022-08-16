#ifndef _CC_OPTIMIZER_H_
#define _CC_OPTIMIZER_H_

#include <vector>
#include <cstddef>

using std::vector;

class CooperativeCoevolutionOptimizer {
public:
	CooperativeCoevolutionOptimizer();
	~CooperativeCoevolutionOptimizer();

	vector<size_t> getActiveComponents();
	void setActiveComponents(vector<size_t>);

protected:
	vector<size_t> activeComponents;
};

#endif
