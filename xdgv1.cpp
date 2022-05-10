#include"xdgv1.hpp"

using std::abs;

string XDGv1::info() {
    return "Extended Differential Grouping version 1 (" + sinfo() + ")";
}

string XDGv1::sinfo() {
    return "XDGv1";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> XDGv1::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    _epsilon = calc_epsilon();
    return XDG::run(ifunc);
}

double XDGv1::calc_epsilon() {
    auto x = vector<vector<double>>(np, vector<double>(func->dim));
    auto xf = vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    auto minf = abs(xf[0]);
    for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
    auto epsilon = alpha * minf;
    return alpha * minf;
}

double XDGv1::epsilon(double y1, double y2, double y3, double y4) {
    return _epsilon;
}
