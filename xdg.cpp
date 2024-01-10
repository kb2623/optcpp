#include "xdg.hpp"

#include "common_funcs.hpp"

#include <limits>

using std::abs;
using std::make_tuple;

// -------------------- XDG algorithm --------------------

XDG::XDG() : AnalAlgorithm() {}

XDG::XDG(const XDG& o) : AnalAlgorithm(o) {}

XDG::~XDG() {}

string XDG::info() {
	return "Extended Differential Grouping (" + sinfo() + ")";
}

string XDG::sinfo() {
	return "XDG";
}

tuple<vector<size_t>, vector<vector<size_t>>> XDG::run(BoundedObjectiveFunction<double>& f) {
	auto allgroups = vector<vector<size_t>>();
	auto seps = vector<size_t>();
	auto DG = vector<vector<size_t>>(f.dim(), vector<size_t>(f.dim(), 0));
	auto group = vector<vector<size_t>>(f.dim(), vector<size_t>(f.dim(), 0));
	auto Num = vector<int>(f.dim(), 1);
	for (size_t i = 0; i < f.dim(); i++) group[i][0] = i;
	auto p1 = new double[f.dim()], p2 = new double[f.dim()], p3 = new double[f.dim()], p4 = new double[f.dim()];
	double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0;
	// Direct Interaction Learning
	for (size_t i = 0; i < f.dim(); i++) {
		for (size_t k = 0; k < f.dim(); k++) p1[k] = p2[k] = f.x_bound_min(k);
		p2[i] = f.x_bound_max(i);
		f1 = f(p1), f2 = f(p2);
		auto delta1 = f1 - f2;
		for (size_t j = i + 1; j < f.dim(); j++) {
			if (DG[i][j] == 0) {
				for (size_t k = 0; k < f.dim(); k++) p3[k] = p1[k], p4[k] = p2[k];
				p3[j] = p4[j] = f.x_bound_min(j) + (f.x_bound_max(j) - f.x_bound_min(j)) / 2;
				f3 = f(p3), f4 = f(p4);
				auto delta2 = f3 - f4;
				if (abs(delta1 - delta2) > epsilon(f1, f2, f3, f4, f.dim())) {
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
	while (sum(Num) > f.dim()) {
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

double XDG::epsilon(double y1, double y2, double y3, double y4, size_t dim) {
	return _epsilon;
}

