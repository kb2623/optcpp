#include"dgv1.hpp"

#include <algorithm>
#include <limits>

using std::abs;
using std::tuple;
using std::vector;

typedef unsigned int uint;

std::string DGv1::info() {
   return "Differential Grouping version 1 (DGv1)";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> DGv1::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    _epsilon = calc_epsilon();
    return DG::run(ifunc);
}

double DGv1::epsilon(double a, double b, double c, double d) {
    return _epsilon;
}

double DGv1::calc_epsilon() {
    auto x = vector<vector<double>>(np, vector<double>(func->dim));
    auto xf = vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    auto minf = abs(xf[0]);
    for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
    auto epsilon = alpha * minf;
    return alpha * minf;
}
