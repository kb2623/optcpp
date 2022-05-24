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
    auto x = vector<vector<double>>(np, vector<double>(func->dim));
    auto xf = vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    auto minf = abs(xf[0]);
    for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
    auto epsilon = alpha * minf;
    return alpha * minf;
}

tuple<vector<uint>, vector<vector<uint>>> DDGv1::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    epsilon_addi = calc_epsilon();
    return DDG::run(ifunc);
}

double DDGv1::get_epsilon_addi(double a, double b, double c, double d) {
    return epsilon_addi;
}

void DDGv1::setParameters(AlgParams *params) {
    this->alpha        = params != nullptr && params->has("alpha") ? params->at<double>("alpha")               : 10e-12;
    this->np           = params != nullptr && params->has("np") ? params->at<size_t>("np")                     : 50;
    this->epsilon_addi = params != nullptr && params->has("epsilon_addi") ? params->at<double>("epsilon_addi") : 1e-12;
}
