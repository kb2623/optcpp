#include "gdg.hpp"

#include "common_funcs.hpp"

#include <algorithm>

using std::abs;
using std::make_tuple;

GDG::GDG() : AnalizeAlgorithm() {}

GDG::~GDG() {}

string GDG::info() {
   return "Global Differential Grouping (" + sinfo() + ")";
}

string GDG::sinfo() {
    return "GDG";
}

tuple<vector<unsigned int>, vector<vector<unsigned int>>> GDG::run(TestFuncBounds* ifunc) {
    initRun(ifunc);
    auto p1 = new double[func->dim], p2 = new double[func->dim], p3 = new double[func->dim], p4 = new double[func->dim];
    for (int i = 0; i < func->dim; i++) p1[i] = func->x_bound_min[i];
    double p1f = eval(p1);
    auto p2_vec = vector<double>(func->dim, 0);
    for (int i = 0; i < func->dim; i++) {
        for (int j = 0; j < func->dim; j++) p2[j] = p1[j];
        p2[i] = func->x_bound_max[i];
        p2_vec[i] = eval(p2);
    }
    auto p3_vec = vector<double>(func->dim, 0);
    for (int i = 0; i < func->dim; i++) {
        for (int j = 0; j < func->dim; j++) p3[j] = p1[j];
        p3[i] = (func->x_bound_max[i] + func->x_bound_min[i]) / 2;
        p3_vec[i] = eval(p3);
    }
    auto deltaMtx = vector<vector<double>>(func->dim, vector<double>(func->dim, 0));
    for (int i = 0; i < func->dim; i++) {
        for (int j = 0; j < func->dim; j++) {
            for (int k = 0; k < func->dim; k++) p4[k] = p1[k];
            p4[i] = func->x_bound_max[i];
            p4[j] = func->x_bound_min[j] + (func->x_bound_max[i] - func->x_bound_min[i]) / 2;
            auto p4f = eval(p4);
            auto d_1 = p1f - p2_vec[i];
            auto d_2 = p3_vec[j] - p4f;
            deltaMtx[i][j] = abs(d_1 - d_2);
            deltaMtx[j][i] = deltaMtx[i][j];
        }
    }
    delete [] p1, delete [] p2, delete [] p3, delete [] p4;
    auto tresh = calc_treshold(deltaMtx);
    auto [labels, rst] = graph_connected_components(deltaMtx, tresh);
    auto group_idx = labels;
    for (int i = -1; i <= *std::max_element(labels.begin(), labels.end()); i++) {
        auto groupsize = std::count(labels.begin(), labels.end(), i);
        if (groupsize == 1) {
            for (int j = 0; j < labels.size(); j++) if (labels[j] == i) group_idx[labels[j]] = -1;
            for (int j = 0; j < labels.size(); j++) if (labels[j] > i) group_idx[labels[j]] -= 1;
        }
    }
    auto seps = find<int, unsigned int>(group_idx, -1);
    auto group_num = *std::max_element(group_idx.begin(), group_idx.end());
    auto allgroups = vector<vector<unsigned int>>();
    for (int i = 0; i <= group_num; i++) {
        auto g = find<int, unsigned int>(group_idx, i);
        if (g.size() != 0) allgroups.push_back(g);
    }
    return make_tuple(seps, allgroups);
}

tuple<vector<int>, vector<int>> GDG::graph_connected_components(vector<vector<double>> &C, const double trash) {
    auto labels = vector<int>(func->dim, -1);
    auto rts = vector<int>();
    int ccc = -1;
    while (true) {
        auto ind = find<int, size_t>(labels, -1);
        if (ind.size() != 0) {
            auto fue = ind[0];
            rts.push_back(fue);
            auto list = vector<size_t>(1, fue);
            ccc++;
            labels[fue] = ccc;
            do {
                auto list_new = vector<size_t>();
                for (int lc = 0; lc < list.size(); lc++) {
                    auto p = list[lc];
                    auto cp = find_tresh(C[p], trash);
                    auto cp1 = vector<size_t>();
                    for (auto e : cp) if (labels[e] == -1) cp1.push_back(e);
                    for (auto e : cp1) labels[e] = ccc;
                    for (auto e : cp1) list_new.push_back(e);
                }
                list = list_new;
            } while (list.size() != 0);
        } else {
            return make_tuple(labels, rts);
        }
    }
}

double GDG::calc_treshold(vector<vector<double>> &deltaMtx) {
    auto x = vector<vector<double>>(np, vector<double>(func->dim));
    auto xf = vector<double>(np);
    for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
    for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    auto minf = abs(xf[0]);
    for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
    return minf * epsilon;
}

vector<size_t> GDG::find_tresh(vector<double> &vec, double tresh) {
    auto r = vector<size_t>();
    for (int i = 0; i < vec.size(); i++) if (vec[i] > tresh) r.push_back(i);
    return r;
}

void GDG::setParameters(AlgParams *params) {
    this->np      = params != nullptr && params->has("np")      ? params->at<size_t>("np")      : 10;
    this->epsilon = params != nullptr && params->has("epsilon") ? params->at<double>("epsilon") : 1e-7;
    this->temp    = params != nullptr && params->has("temp")    ? params->at<double>("temp")    : .5;
}
