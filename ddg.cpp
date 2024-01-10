#include "ddg.hpp"

#include "common_funcs.hpp"

using std::log;
using std::abs;
using std::find;
using std::make_tuple;

DDG::DDG() : AnalAlgorithm() {}

DDG::DDG(const DDG& o) : AnalAlgorithm(o) {}

DDG::~DDG() {}

string DDG::info() {
	return "Dual Differential Grouping (" + sinfo() + ")";
}

string DDG::sinfo() {
	return "DDG";
}

tuple<vector<size_t>, vector<vector<size_t>>> DDG::run(BoundedObjectiveFunction<double>& fitf) {
	auto dims = vector<size_t>(fitf.dim());
	for (size_t i = 0; i < fitf.dim(); i++) dims[i] = i;
	auto seps = vector<size_t>();
	auto allgroups = vector<vector<size_t>>();
	auto x1 = new double[fitf.dim()], x2 = new double[fitf.dim()], x3 = new double[fitf.dim()], x4 = new double[fitf.dim()];
	for (size_t i = 0; i < fitf.dim(); i++) x1[i] = fitf.x_bound_min(i);
	auto y1 = fitf(x1), y2 = .0, y3 = .0, y4 = .0;
	auto delta_addi = .0, delta_multi = .0;
	while (dims.size() > 0) {
		auto i = dims[0];
		auto tmpgroup = vector<size_t>(1, i);
		for (size_t j = 0; j < fitf.dim(); j++) x2[i] = x1[i];
		x2[i] = fitf.x_bound_max(i);
		y2 = fitf(x2);
		for (auto j : dims) if (i != j) {
			for (size_t k = 0; k < fitf.dim(); k++) x3[k] = x1[k];
			x3[j] = fitf.x_bound_min(j) + (fitf.x_bound_max(j) - fitf.x_bound_min(j)) / 2;
			y3 = fitf(x3);
			for(size_t k = 0; k < fitf.dim(); k++) x4[k] = x2[k];
			x4[j] = fitf.x_bound_min(j) + (fitf.x_bound_max(j) - fitf.x_bound_min(j)) / 2;
			y4 = fitf(x4);
			delta_addi = abs((y1 - y2) - (y3 - y4));
			if (y1 > 0 && y2 > 0 && y3 > 0 && y4 > 0) {
				delta_multi = abs((log(y1) - log(y2)) - (log(y3) - log(y4)));
			} else {
				delta_multi = epsilon_multi + 1;
			}
			double epsilon_addi = get_epsilon_addi(y1, y2, y3, y4, fitf);
			if (delta_addi > epsilon_addi && delta_multi > epsilon_multi) {
				tmpgroup.push_back(j);
			}
		}
		if (tmpgroup.size() == 1) {
			seps = vunion(seps, tmpgroup);
		} else {
			allgroups.push_back(tmpgroup);
		}
		for (size_t j = 0; j < tmpgroup.size(); j++) {
			auto k = find(dims, tmpgroup[j]);
			if (k > -1) dims.erase(dims.begin() + k);
		}
	}
	delete[] x1, delete[] x2, delete[] x3, delete[] x4;
	return make_tuple(seps, allgroups);
}

double DDG::get_epsilon_addi(double y1, double y2, double y3, double y4, BoundedObjectiveFunction<double>& fitf) {
	return epsilon_addi;
}

