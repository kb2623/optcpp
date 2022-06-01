#include "rdg.hpp"

#include "common_funcs.hpp"

using std::abs;
using std::make_tuple;

RDG::RDG() : AnalizeAlgorithm() {}

RDG::~RDG() {}

string RDG::info() {
	return "Recursive Differential Grouping (" + sinfo() + ")";
}

string RDG::sinfo() {
	return "RDG";
}

void RDG::setParameters(AlgParams *params) {
	this->np    = getParam(params, "np", 10);
	this->alpha = getParam(params, "alpha", 10e-12);
}

double RDG::calc_epsilon(TestFuncBounds* func) {
	auto x = vector<vector<double>>(np, vector<double>(func->dim));
	auto xf = vector<double>(np);
	for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
	for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
	auto minf = abs(xf[0]);
	for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
	return alpha * minf;
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> RDG::run(TestFuncBounds* ifunc) {
	initRun(ifunc);
	auto epsilon = calc_epsilon(func);
	auto x = new double[func->dim];
	for (int i = 0; i < func->dim; i++) x[i] = func->x_bound_min[i];
	auto xf = eval(x);
	auto sub1 = vector<unsigned int>(1, 0), sub2 = vector<unsigned int>(func->dim - 1);
	for (int i = 0; i < sub2.size(); i++) sub2[i] = i + 1;
	auto xremain = vector<unsigned int>(1, 0);
	auto seps = vector<unsigned int>();
	auto nongroups = vector<vector<unsigned int>>();
	while (xremain.size() > 0) {
		xremain = vector<unsigned int>();
		auto sub1_a = interact(x, xf, epsilon, sub1, sub2, xremain);
		if (sub1_a.size() == sub1.size()) {
			if (sub1.size() == 1) {
				seps.insert(seps.end(), sub1.begin(), sub1.end());
			} else {
				nongroups.push_back(sub1);
			}
			if (xremain.size() > 1) {
				sub1 = vector<unsigned int>(1, xremain[0]);
				xremain.erase(xremain.begin());
				sub2 = xremain;
			} else {
				seps.push_back(xremain[0]);
				break;
			}
		} else {
			sub1 = sub1_a;
			sub2 = xremain;
			if (xremain.size() == 0) {
				nongroups.push_back(sub1);
				break;
			}
		}
	}
	delete[] x;
	return make_tuple(seps, nongroups);
}

vector<unsigned int> RDG::interact(double *a, double af, double epsilon, vector<unsigned int> sub1, vector<unsigned int> sub2, vector<unsigned int> &xremain) {
	auto b = new double[func->dim], c = new double[func->dim], d = new double[func->dim];
	for (int i = 0; i < func->dim; i++) b[i] = c[i] = a[i];
	for (int i = 0; i < sub1.size(); i++) b[sub1[i]] = func->x_bound_max[sub1[i]];
	auto bf = eval(b);
	auto delta1 = af - bf;
	for (int i = 0; i < func->dim; i++) d[i] = b[i];
	for (int i = 0; i < sub2.size(); i++) c[sub2[i]] = d[sub2[i]] = func->x_bound_min[sub2[i]] + (func->x_bound_max[sub2[i]] - func->x_bound_min[sub2[i]]) / 2;
	auto cf = eval(c), df = eval(d);
	auto delta2 = cf - df;
	delete[] b, delete[] c, delete[] d;
	if (abs(delta1 - delta2) > epsilon) {
		if (sub2.size() == 1) {
			sub1 = vunion(sub1, sub2);
		} else {
			unsigned int k = floor(sub2.size() / 2);
			auto sub2_1 = vector<unsigned int>();
			for (int i = 0; i < k; i++) sub2_1.push_back(sub2[i]);
			auto sub2_2 = vector<unsigned int>();
			for (int i = 0; i < sub2.size() - k; i++) sub2_2.push_back(sub2[k + i]);
			auto sub1_1 = interact(a, af, epsilon, sub1, sub2_1, xremain);
			auto sub1_2 = interact(a, af, epsilon, sub1, sub2_2, xremain);
			sub1 = vunion(sub1_1, sub1_2);
		}
	} else {
		xremain.insert(xremain.end(), sub2.begin(), sub2.end());
	}
	return sub1;
}
