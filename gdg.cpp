#include"gdg.hpp"

#include <algorithm>

using std::abs;
using std::find;
using std::make_tuple;

string GDG::info() {
   return "Global Differential Grouping (" + sinfo() + ")";
}

string GDG::sinfo() {
    return "GDG";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> GDG::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    auto seps = vector<uint>();
    auto allgroups = vector<vector<uint>>();
    auto dims = vector<uint>(func->dim);
    for (uint i = 0; i < func->dim; i++) dims[i] = i;
    auto p1 = new double[func->dim], p2 = new double[func->dim];
    while (dims.size() > 0) {
        auto i = dims[0];
        auto group = vector<uint>(1, i);
        for (auto j : dims) if (i != j) {
            for (uint k = 0; k < func->dim; k++) p1[k] = p2[k] = func->x_bound_min[k];
            p2[i] = func->x_bound_max[i];
            auto y1 = eval(p1), y2 = eval(p2);
            auto delta1 = y1 - y2;
            p1[j] = p2[j] = (func->x_bound_min[j] + func->x_bound_max[j]) / 2;
            auto y3 = eval(p1), y4 = eval(p2);
            auto delta2 = y3 - y4;
        }
        for (auto e : group) {
            auto it = find(dims.begin(), dims.end(), e);
            if (it != dims.end()) dims.erase(it);
        }
        if (group.size() == 1) {
            seps.push_back(group[0]);
        } else {
            allgroups.push_back(group);
        }
    }
    delete[] p1, delete[] p2;
    return make_tuple(seps, allgroups);
}

void GDG::graph_connected_components() {
    // TODO
}

double GDG::calc_treshold() {
    auto x = vector<vector<double>>(np, vector<double>(func->dim));
    auto xf = vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    auto minf = abs(xf[0]);
    for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
    return minf * epsilon;
}
