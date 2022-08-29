#include"ddgv2.hpp"

DDGv2::DDGv2() {}

DDGv2::~DDGv2() {}

string DDGv2::info() {
	return "Dual Differential Grouping version 2 (" + sinfo() + ")";
}

string DDGv2::sinfo() {
	return "DDGv2";
}

double DDGv2::get_epsilon_addi(double y1, double y2, double y3, double y4) {
	return gamma(pow(fitf.dim(), 0.5)) * (y1 + y2 + y3 + y4);
}

double DDGv2::gamma(double n) {
	double mu = n * eps;
	return mu / (1 - mu);
}
