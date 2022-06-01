#include "xdg.hpp"

#include "common_funcs.hpp"

#include <algorithm>
#include <limits>

using std::abs;
using std::make_tuple;

XDG::XDG() : AnalizeAlgorithm() {}

XDG::~XDG() {}

string XDG::info() {
	return "Extended Differential Grouping (" + sinfo() + ")";
}

string XDG::sinfo() {
	return "XDG";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> XDG::run(TestFuncBounds* ifunc) {
	initRun(ifunc);
	auto allgroups = vector<vector<uint>>();
	auto seps = vector<uint>();
	auto DG = vector<vector<uint>>(func->dim, vector<uint>(func->dim, 0));
	auto group = vector<vector<uint>>(func->dim, vector<uint>(func->dim, 0));
	auto Num = vector<int>(func->dim, 1);
	for (uint i = 0; i < func->dim; i++) group[i][0] = i;
	auto p1 = new double[func->dim], p2 = new double[func->dim], p3 = new double[func->dim], p4 = new double[func->dim];
	double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0;
	// Direct Interaction Learning
	for (uint i = 0; i < func->dim; i++) {
		for (uint k = 0; k < func->dim; k++) p1[k] = p2[k] = func->x_bound_min[k];
		p2[i] = func->x_bound_max[i];
		f1 = eval(p1), f2 = eval(p2);
		auto delta1 = f1 - f2;
		for (uint j = i + 1; j < func->dim; j++) {
			if (DG[i][j] == 0) {
				for (uint k = 0; k < func->dim; k++) p3[k] = p1[k], p4[k] = p2[k];
				p3[j] = p4[j] = func->x_bound_min[j] + (func->x_bound_max[j] - func->x_bound_min[j]) / 2;
				f3 = eval(p3), f4 = eval(p4);
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
	while (sum(Num) > func->dim) {
		int Num_groups = group.size();
		for (int i = 0; i < Num_groups - 1; i++) {
			for (int j = i + 1; j < Num_groups;) {
				auto t1 = vector<uint>(group[i].begin(), group[i].begin() + Num[i]);
				auto t2 = vector<uint>(group[j].begin(), group[j].begin() + Num[j]);
				if (have_intersection<uint>(t1, t2)) {
					auto g1 = vunion(t1, t2);
					Num[i] = g1.size();
					for (uint k = 0; k < Num[i]; k++) group[i][k] = g1[k];
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
	for (uint i = 0; i < group.size(); i++) if (Num[i] == 1) {
		seps.push_back(group[i][0]);
	} else {
		allgroups.push_back(vector<uint>(group[i].begin(), group[i].begin() + Num[i]));
	}
	delete[] p1, delete[] p2, delete[] p3, delete[] p4;
	return make_tuple(seps, allgroups);
}

double XDG::epsilon(double y1, double y2, double y3, double y4) {
	return _epsilon;
}

void XDG::setParameters(AlgParams *params) {
	this->_epsilon = getParam(params, "epsilon", 10e-12);
}
