#include "ddgv1.hpp"

using std::abs;

DDGv1::DDGv1() : DDG() {}


DDGv1::DDGv1(const DDGv1& o) : DDG(o) {}

DDGv1::~DDGv1() {}

string DDGv1::sinfo() {
	return "DDGv1";
}

string DDGv1::info() {
	return "Dual Differential Grouping version 1 (" + sinfo() + ")";
}

double DDGv1::calc_epsilon(BoundedObjectiveFunction<double>& fitf) {
	auto x = vector<vector<double>>(np, vector<double>(fitf.dim()));
	auto xf = vector<double>(np);
	std::uniform_real_distribution<double> dist(0.0,1.0);
	for (int i = 0; i < np; i++) for (int j = 0; j < fitf.dim(); j++) x[i][j] = fitf.x_bound_max(j) - fitf.x_bound_min(j) * dist(rand_gen) + fitf.x_bound_min(j);
	for (int i = 0; i < np; i++) xf[i] = fitf(x[i].data());
	auto minf = abs(xf[0]);
	for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
	auto epsilon = alpha * minf;
	return alpha * minf;
}

tuple<vector<size_t>, vector<vector<size_t>>> DDGv1::run(BoundedObjectiveFunction<double>& ifunc) {
	epsilon_addi = calc_epsilon(ifunc);
	return DDG::run(ifunc);
}

double DDGv1::get_epsilon_addi(double a, double b, double c, double d, BoundedObjectiveFunction<double>& fitf) {
	return epsilon_addi;
}

