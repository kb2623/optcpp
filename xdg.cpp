#include "xdg.hpp"

#include "common_funcs.hpp"

#include <algorithm>
#include <limits>

using std::abs;
using std::make_tuple;

// -------------------- XDG algorithm params --------------------

XDGParams::XDGParams() : RunAlgParams() {}

XDGParams::XDGParams(BoundedObjectiveFunction<double>& fitf) : RunAlgParams(fitf) {}

XDGParams::~XDGParams() {}

// -------------------- XDG algorithm --------------------

XDG::XDG() : AnalizeAlgorithm() {}

XDG::~XDG() {}

string XDG::info() {
	return "Extended Differential Grouping (" + sinfo() + ")";
}

string XDG::sinfo() {
	return "XDG";
}

RunAlgParams<double>& XDG::initRun(thread_data& tdata, BoundedObjectiveFunction<double>& func) {
	auto r = XDGParams(func);
	return r;
}

tuple<vector<size_t>, vector<vector<size_t>>> XDG::run(BoundedObjectiveFunction<double>& ifunc) {
	auto tdata_main = thread_data();
	auto params = initRun(tdata_main, ifunc);
	auto allgroups = vector<vector<size_t>>();
	auto seps = vector<size_t>();
	auto DG = vector<vector<size_t>>(params.fitf().dim(), vector<size_t>(params.fitf().dim(), 0));
	auto group = vector<vector<size_t>>(params.fitf().dim(), vector<size_t>(params.fitf().dim(), 0));
	auto Num = vector<int>(params.fitf().dim(), 1);
	for (size_t i = 0; i < params.fitf().dim(); i++) group[i][0] = i;
	auto p1 = new double[params.fitf().dim()], p2 = new double[params.fitf().dim()], p3 = new double[params.fitf().dim()], p4 = new double[params.fitf().dim()];
	double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0;
	// Direct Interaction Learning
	for (size_t i = 0; i < params.fitf().dim(); i++) {
		for (size_t k = 0; k < params.fitf().dim(); k++) p1[k] = p2[k] = params.fitf().x_bound_min(k);
		p2[i] = params.fitf().x_bound_max(i);
		f1 = params.fitf()(p1), f2 = params.fitf()(p2);
		auto delta1 = f1 - f2;
		for (size_t j = i + 1; j < params.fitf().dim(); j++) {
			if (DG[i][j] == 0) {
				for (size_t k = 0; k < params.fitf().dim(); k++) p3[k] = p1[k], p4[k] = p2[k];
				p3[j] = p4[j] = params.fitf().x_bound_min(j) + (params.fitf().x_bound_max(j) - params.fitf().x_bound_min(j)) / 2;
				f3 = params.fitf()(p3), f4 = params.fitf()(p4);
				auto delta2 = f3 - f4;
				if (abs(delta1 - delta2) > epsilon(f1, f2, f3, f4)) {
					DG[i][j] = 1;
					Num[i]++;
					group[i][Num[i] - 1] = j;
				}
			} else {
				Num[i]++;
				group[i][Num[i] - 1] = j;
			}
		}
		if (Num[i] > 2) for (int p = 1; p < Num[i] - 1; p++) for (int q = p + 1; q < Num[i]; q++) DG[group[i][p]][group[i][q]] = 1;
	}
	// Indirect Interaction Learning
	while (sum(Num) > params.fitf().dim()) {
		int Num_groups = group.size();
		for (int i = 0; i < Num_groups - 1; i++) {
			for (int j = i + 1; j < Num_groups;) {
				auto t1 = vector<size_t>(group[i].begin(), group[i].begin() + Num[i]);
				auto t2 = vector<size_t>(group[j].begin(), group[j].begin() + Num[j]);
				if (have_intersection<size_t>(t1, t2)) {
					auto g1 = vunion(t1, t2);
					Num[i] = g1.size();
					for (size_t k = 0; k < Num[i]; k++) group[i][k] = g1[k];
					group.erase(group.begin() + j);
					Num.erase(Num.begin() + j);
					Num_groups--;
				} else {
					j++;
				}
			}
		}
	}
	// Separable Variable Grouping
	for (size_t i = 0; i < group.size(); i++) if (Num[i] == 1) {
		seps.push_back(group[i][0]);
	} else {
		allgroups.push_back(vector<size_t>(group[i].begin(), group[i].begin() + Num[i]));
	}
	delete[] p1, delete[] p2, delete[] p3, delete[] p4;
	return make_tuple(seps, allgroups);
}

double XDG::epsilon(double y1, double y2, double y3, double y4) {
	return _epsilon;
}

void XDG::setParameters(AlgParams& params) {
	AnalizeAlgorithm::setParameters(params);
	this->_epsilon = getParam(params, "epsilon", 10e-12);
}
