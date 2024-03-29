#include "gdg.hpp"

#include "common_funcs.hpp"

#include <algorithm>

using std::abs;
using std::make_tuple;

GDG::GDG() : AnalAlgorithm(), np(50) {}

GDG::GDG(const GDG& o) : AnalAlgorithm(o), np(o.np) {}

GDG::~GDG() {}

string GDG::info() {
	return "Global Differential Grouping (" + sinfo() + ")";
}

string GDG::sinfo() {
	return "GDG";
}

tuple<vector<size_t>, vector<vector<size_t>>> GDG::run(BoundedObjectiveFunction<double>& fitf) {
	auto p1 = new double[fitf.dim()], p2 = new double[fitf.dim()], p3 = new double[fitf.dim()], p4 = new double[fitf.dim()];
	for (int i = 0; i < fitf.dim(); i++) p1[i] = fitf.x_bound_min(i);
	double p1f = fitf(p1);
	auto p2_vec = vector<double>(fitf.dim(), 0);
	for (int i = 0; i < fitf.dim(); i++) {
		for (int j = 0; j < fitf.dim(); j++) p2[j] = p1[j];
		p2[i] = fitf.x_bound_max(i);
		p2_vec[i] = fitf(p2);
	}
	auto p3_vec = vector<double>(fitf.dim(), 0);
	for (int i = 0; i < fitf.dim(); i++) {
		for (int j = 0; j < fitf.dim(); j++) p3[j] = p1[j];
		p3[i] = (fitf.x_bound_max(i) + fitf.x_bound_min(i)) / 2;
		p3_vec[i] = fitf(p3);
	}
	auto deltaMtx = vector<vector<double>>(fitf.dim(), vector<double>(fitf.dim(), 0));
	for (int i = 0; i < fitf.dim(); i++) {
		for (int j = 0; j < fitf.dim(); j++) {
			for (int k = 0; k < fitf.dim(); k++) p4[k] = p1[k];
			p4[i] = fitf.x_bound_max(i);
			p4[j] = fitf.x_bound_min(j) + (fitf.x_bound_max(i) - fitf.x_bound_min(i)) / 2;
			auto p4f = fitf(p4);
			auto d_1 = p1f - p2_vec[i];
			auto d_2 = p3_vec[j] - p4f;
			deltaMtx[i][j] = abs(d_1 - d_2);
			deltaMtx[j][i] = deltaMtx[i][j];
		}
	}
	delete [] p1, delete [] p2, delete [] p3, delete [] p4;
	auto tresh = calc_treshold(deltaMtx, fitf);
	auto [labels, rst] = graph_connected_components(deltaMtx, tresh, fitf);
	auto group_idx = labels;
	for (int i = -1; i <= *std::max_element(labels.begin(), labels.end()); i++) {
		auto groupsize = std::count(labels.begin(), labels.end(), i);
		if (groupsize == 1) {
			for (int j = 0; j < labels.size(); j++) if (labels[j] == i) group_idx[labels[j]] = -1;
			for (int j = 0; j < labels.size(); j++) if (labels[j] > i) group_idx[labels[j]] -= 1;
		}
	}
	auto seps = find<int, size_t>(group_idx, -1);
	auto group_num = *std::max_element(group_idx.begin(), group_idx.end());
	auto allgroups = vector<vector<size_t>>();
	for (int i = 0; i <= group_num; i++) {
		auto g = find<int, size_t>(group_idx, i);
		if (g.size() != 0) allgroups.push_back(g);
	}
	return make_tuple(seps, allgroups);
}

tuple<vector<int>, vector<int>> GDG::graph_connected_components(vector<vector<double>> &C, const double trash, BoundedObjectiveFunction<double>& fitf) {
	auto labels = vector<int>(fitf.dim(), -1);
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

double GDG::calc_treshold(vector<vector<double>> &deltaMtx, BoundedObjectiveFunction<double>& fitf) {
	auto x = vector<vector<double>>(np, vector<double>(fitf.dim()));
	auto xf = vector<double>(np);
	std::uniform_real_distribution<double> dist(0.0,1.0);
	for (int i = 0; i < np; i++) for (int j = 0; j < fitf.dim(); j++) x[i][j] = fitf.x_bound_max(j) - fitf.x_bound_min(j) * dist(rand_gen) + fitf.x_bound_min(j);
	for (int i = 0; i < np; i++) xf[i] = fitf(x[i].data());
	auto minf = abs(xf[0]);
	for (int i = 1; i < np; i++) if (minf > abs(xf[i])) minf = abs(xf[i]);
	return minf * epsilon;
}

vector<size_t> GDG::find_tresh(vector<double> &vec, double tresh) {
	auto r = vector<size_t>();
	for (int i = 0; i < vec.size(); i++) if (vec[i] > tresh) r.push_back(i);
	return r;
}

