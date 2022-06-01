#ifndef _CC_SEARCH_ALGORITHM_H_
#define _CC_SEARCH_ALGORITHM_H_

#include "search_algorithm.hpp"

class CooperativeCoevolution : public SearchAlgorithm {
public:
	CooperativeCoevolution();
	~CooperativeCoevolution();

	virtual void run_iteration() = 0;
	virtual vector<size_t> getActiveComponents() = 0;
};

#endif
