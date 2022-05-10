#include"dg.hpp"

#include <algorithm>

using std::abs;
using std::find;
using std::make_tuple;

string DG::info() {
   return "Differential Grouping (" + sinfo() + ")";
}

string DG::sinfo() {
    return "DG";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> DG::run(TestFuncBounds* ifunc) {
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
            if (abs(delta1 - delta2) > epsilon(y1, y2, y3, y4)) group.push_back(j);
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

double DG::epsilon(double a, double b, double c, double d) {
    return 1e-12;
}
