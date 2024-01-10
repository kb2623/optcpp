#include"gdgv1.hpp"

#include <algorithm>

using std::abs;
using std::make_tuple;

GDGv1::GDGv1() : GDG() {}

GDGv1::GDGv1(const GDGv1& o) : GDG(o) {}

GDGv1::~GDGv1() {}

string GDGv1::info() {
	return "Global Differential Grouping version 1 (" + sinfo() + ")";
}

string GDGv1::sinfo() {
	return "GDGv1";
}

double GDGv1::calc_treshold(vector<vector<double>> &deltaMtx) {
	auto min_e = std::numeric_limits<double>::max();
	for (auto vv : deltaMtx) for (auto v : vv) min_e = std::min(min_e, v);
	return min_e;
}
