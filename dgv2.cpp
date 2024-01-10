#include"dgv2.hpp"

#include <algorithm>
#include <limits>

using std::abs;
using std::tuple;
using std::vector;
using std::make_tuple;

DGv2::DGv2() {}

DGv2::DGv2(const DGv2& o) : DG(o) {}

DGv2::~DGv2() {}

std::string DGv2::info() {
   return "Differential Grouping version 2 (DGv2)";
}

double DGv2::epsilon(double y1, double y2, double y3, double y4) {
    return gamma(pow(func->dim, 0.5)) * (y1 + y2 + y3 + y4);
}

double DGv2::gamma(double n) {
    double mu = n * eps;
    return mu / (1 - mu);
}
