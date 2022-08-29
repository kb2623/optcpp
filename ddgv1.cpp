#include"ddgv1.hpp"

using std::abs;

DDGv1::DDGv1() : DDG() {}

DDGv1::~DDGv1() {}

string DDGv1::sinfo() {
	return "DDGv1";
}

string DDGv1::info() {
	return "Dual Differential Grouping version 1 (" + sinfo() + ")";
}

double DDGv1::calc_epsilon() {
	auto x = vector<vector<double>>(np, vector<double>(fitf.dim()));
	auto xf = vector<double>(np);
	for (int i = 0; i < np; i++) for (int j = 0; j < fitf.dim(); j++) x[i][j] = fitf.x_bound_max()[j] - fitf.x_bound_min()[j] * randDouble() + fitf.x_bound_min()[j];
	for (int i = 0; i < np; i++) xf[i] = fitf(x[i].data());
	auto minf = abs(xf[0]);
	for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
	auto epsilon = alpha * minf;
	return alpha * minf;
}

tuple<vector<uint>, vector<vector<uint>>> DDGv1::run(BoundedObjectiveFunction<double>* ifunc) {
	initRun(ifunc);
	epsilon_addi = calc_epsilon();
	return DDG::run(ifunc);
}

double DDGv1::get_epsilon_addi(double a, double b, double c, double d) {
	return epsilon_addi;
}

void DDGv1::setParameters(AlgParams *params) {
	this->alpha        = getParam(params, "alpha",        10e-12);
	this->np           = getParam(params, "np",           50);
	this->epsilon_addi = getParam(params, "epsilon_addi", 1e-12);
}
