#include"gdgv2.hpp"

#include <algorithm>

using std::abs;
using std::make_tuple;

string GDGv2::info() {
   return "Global Differential Grouping version 2 (" + sinfo() + ")";
}

string GDGv2::sinfo() {
    return "GDGv2";
}

double GDGv2::calc_treshold(vector<vector<double>> &deltaMtx) {
    auto min_e = std::numeric_limits<double>::max();
    for (auto vv : deltaMtx) for (auto v : vv) min_e = std::min(min_e, v);
    double mu = (pow(func->dim, 0.5) * min_e) * epsilon;
    return mu / (1 - mu);
}
