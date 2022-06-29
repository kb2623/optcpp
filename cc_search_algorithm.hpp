#ifndef _CC_SEARCH_ALGORITHM_H_
#define _CC_SEARCH_ALGORITHM_H_

#include "search_algorithm.hpp"
#include "analize_algorithm.hpp"

class CooperativeCoevolutionAlgorithm : public SearchAlgorithm {
public:
	CooperativeCoevolutionAlgorithm();
	~CooperativeCoevolutionAlgorithm();

private:
	vector<SearchAlgorithm*> algs;
	AnalizeAlgorithm* anal;

};

#endif
