#include"xdgv2.hpp"

XDGv2::XDGv2() {}

XDGv2::~XDGv2() {}

string XDGv2::info() {
	return "Extended Differential Grouping version 2 (" + sinfo() + ")";
}

string XDGv2::sinfo() {
	return "XDGv2";
}

double XDGv2::gamma(double n) {
	double mu = n * eps;
	return mu / (1 - mu);
}

double XDGv2::epsilon(double y1, double y2, double y3, double y4) {
	return gamma(pow(func->dim, 0.5)) * (y1 + y2 + y3 + y4);
}
