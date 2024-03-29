#include"dg.hpp"

#include <algorithm>

using std::abs;
using std::find;
using std::make_tuple;

DG::DG() : AnalAlgorithm() {}

DG::DG(const DG& o) : AnalAlgorithm(o) {}

DG::~DG() {}

string DG::info() {
	return "Differential Grouping (" + sinfo() + ")";
}

string DG::sinfo() {
	return "DG";
}

tuple<vector<size_t>, vector<vector<size_t>>> DG::run(BoundedObjectiveFunction<double>& fitf) {
	auto seps = vector<size_t>();
	auto allgroups = vector<vector<size_t>>();
	auto dims = vector<size_t>(fitf.dim());
	for (size_t i = 0; i < fitf.dim(); i++) dims[i] = i;
	auto p1 = new double[fitf.dim()], p2 = new double[fitf.dim()];
	while (dims.size() > 0) {
		auto i = dims[0];
		auto group = vector<size_t>(1, i);
		for (auto j : dims) if (i != j) {
			for (size_t k = 0; k < fitf.dim(); k++) p1[k] = p2[k] = fitf.x_bound_min(k);
			p2[i] = fitf.x_bound_max(i);
			auto y1 = fitf(p1), y2 = fitf(p2);
			auto delta1 = y1 - y2;
			p1[j] = p2[j] = fitf.x_bound_min(j) + (fitf.x_bound_max(j) - fitf.x_bound_min(j)) / 2;
			auto y3 = fitf(p1), y4 = fitf(p2);
			auto delta2 = y3 - y4;
			if (abs(delta1 - delta2) > epsilon(y1, y2, y3, y4)) group.push_back(j);
		}
		for (auto e : group) {
			auto it = find(dims.begin(), dims.end(), e);
			if (it != dims.end()) dims.erase(it);
		}
		if (group.size() == 1) {
			seps.push_back(group[0]);
		} else {
			allgroups.push_back(group);
		}
	}
	delete[] p1, delete[] p2;
	return make_tuple(seps, allgroups);
}

double DG::epsilon(double a, double b, double c, double d) {
	return 1e-12;
}

