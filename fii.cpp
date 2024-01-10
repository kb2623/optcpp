#include "fii.hpp"

using std::abs;
using std::make_tuple;

FII::FII() : AnalAlgorithm() {}

FII::FII(const FII& o) : AnalAlgorithm(o) {}

FII::~FII() {}

string FII::info() {
	return "Fast Interdependency Identification (" + sinfo() + ")";
}

string FII::sinfo() {
	return "FII";
}

tuple<vector<size_t>, vector<vector<size_t>>> FII::run(BoundedObjectiveFunction<double>& ifunc) {
	auto seps = vector<size_t>();
	auto allgroups = vector<vector<size_t>>();
	// TODO
	return make_tuple(seps, allgroups);
}
