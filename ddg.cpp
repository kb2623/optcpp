#include"ddg.hpp"

using std::log;
using std::abs;
using std::find;
using std::make_tuple;

string DDG::info() {
    return "Dual Differential Grouping (" + sinfo() + ")";
}

string DDG::sinfo() {
    return "DDG";
}

tuple<vector<uint>, vector<vector<uint>>> DDG::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    auto dims = vector<uint>(func->dim);
    for (uint i = 0; i < func->dim; i++) dims[i] = i;
    auto seps = vector<uint>();
    auto allgroups = vector<vector<uint>>();
    auto x1 = new double[func->dim], x2 = new double[func->dim], x3 = new double[func->dim], x4 = new double[func->dim];
    for (uint i = 0; i < func->dim; i++) x1[i] = func->x_bound_min[i];
    auto y1 = eval(x1), y2 = .0, y3 = .0, y4 = .0;
    auto delta_addi = .0, delta_multi = .0;
    while (dims.size() > 0) {
        auto i = dims[0];
        auto tmpgroup = vector<uint>(1, i);
        for (uint j = 0; j < func->dim; j++) x2[i] = x1[i];
        x2[i] = func->x_bound_max[i];
        y2 = eval(x2);
        for (auto j : dims) if (i != j) {
            for (uint k = 0; k < func->dim; k++) x3[k] = x1[k];
            x3[j] = (func->x_bound_min[j] + func->x_bound_max[j]) / 2;
            y3 = eval(x3);
            for(uint k = 0; k < func->dim; k++) x4[k] = x2[k];
            x4[j] = (func->x_bound_min[j] - func->x_bound_max[j]) / 2;
            y4 = eval(x4);
            delta_addi = abs((y1 - y2) - (y3 - y4));
            if (y1 > 0 && y2 > 0 && y3 > 0 && y4 > 0) {
                delta_multi = abs((log(y1) - log(y2)) - (log(y3) - log(y4)));
            } else {
                delta_multi = epsilon_multi + 1;
            }
            double epsilon_addi = get_epsilon_addi(y1, y2, y3, y4);
            if (delta_addi > epsilon_addi && delta_multi > epsilon_multi) {
                tmpgroup.push_back(j);
            }
        }
        if (tmpgroup.size() == 1) {
            seps = vuinon(seps, tmpgroup);
        } else {
            allgroups.push_back(tmpgroup);
        }
        for (uint j = 0; j < tmpgroup.size(); j++) {
            auto k = find(dims, tmpgroup[j]);
            if (k > -1) dims.erase(dims.begin() + k);
        }
    }
    delete[] x1, delete[] x2, delete[] x3, delete[] x4;
    return make_tuple(seps, allgroups);
}

double DDG::get_epsilon_addi(double y1, double y2, double y3, double y4) {
    return 1e-3;
}
