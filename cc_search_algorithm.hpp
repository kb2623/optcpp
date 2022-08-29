#ifndef _CC_SEARCH_ALGORITHM_H_
#define _CC_SEARCH_ALGORITHM_H_

#include "search_algorithm.hpp"
#include "analize_algorithm.hpp"

template<typename T>
class CooperativeCoevolutionAlgorithm : public SearchAlgorithm<T> {
public:
	CooperativeCoevolutionAlgorithm();
	~CooperativeCoevolutionAlgorithm();

private:
	vector<SearchAlgorithm<T>*> algs;
	AnalizeAlgorithm<T>* anal;

};

#endif
