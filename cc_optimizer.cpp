#include "cc_optimizer.hpp"

CooperativeCoevolutionOptimizer::CooperativeCoevolutionOptimizer() {}

CooperativeCoevolutionOptimizer::~CooperativeCoevolutionOptimizer() {}

vector<size_t> CooperativeCoevolutionOptimizer::getActiveComponents() {
	return activeComponents;
}

void CooperativeCoevolutionOptimizer::setActiveComponents(vector<size_t> newActiveComponets) {
	this->activeComponents = newActiveComponets;
}
