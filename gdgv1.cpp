#include"gdgv1.hpp"

#include <algorithm>

using std::abs;
using std::make_tuple;

GDGv1::GDGv1() : GDG(10, 10e-10) {}

GDGv1::GDGv1(size_t np, double epsilon) : GDG(np, epsilon) {}

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
