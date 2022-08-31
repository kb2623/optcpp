#include "fii.hpp"

using std::abs;
using std::make_tuple;

FII::FII() : AnalizeAlgorithm() {}

FII::~FII() {}

string FII::info() {
	return "Fast Interdependency Identification (" + sinfo() + ")";
}

string FII::sinfo() {
	return "FII";
}

void FII::setParameters(AlgParams *params) {
	// TODO
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> FII::run(BoundedObjectiveFunction<double>* ifunc) {
	// TODO
}
