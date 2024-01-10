#include "rdgv2.hpp"

#include "common_funcs.hpp"

#include <algorithm>
#include <limits>

using std::abs;
using std::pow;
using std::make_tuple;

RDGv2::RDGv2() {}

RDGv2::RDGv2(const RDGv2& o) : RDG(o) {}

RDGv2::~RDGv2() {}

string RDGv2::info() {
	return "Recursive Differential Grouping version 2 (" + sinfo() + ")";
}

string RDGv2::sinfo() {
	return "RDG2";
}

tuple<vector<size_t>, vector<vector<size_t>>> RDGv2::run(BoundedObjectiveFunction<double>& fitf) {
	auto x = new double[fitf.dim()];
	for (int i = 0; i < fitf.dim(); i++) x[i] = fitf.x_bound_min(i);
	auto xf = fitf(x);
	auto sub1 = vector<size_t>(1, 0), sub2 = vector<size_t>(fitf.dim() - 1);
	for (int i = 0; i < sub2.size(); i++) sub2[i] = i + 1;
	auto xremain = vector<size_t>(1, 0);
	auto seps = vector<size_t>();
	auto nongroups = vector<vector<size_t>>();
	while (xremain.size() > 0) {
		xremain = vector<size_t>();
		auto sub1_a = interact(x, xf, sub1, sub2, xremain, fitf);
		if (sub1_a.size() == sub1.size()) {
			if (sub1.size() == 1) {
				seps.insert(seps.end(), sub1.begin(), sub1.end());
			} else {
				nongroups.push_back(sub1);
			}
			if (xremain.size() > 1) {
				sub1 = vector<size_t>(1, xremain[0]);
				xremain.erase(xremain.begin());
				sub2 = xremain;
			} else {
				seps.push_back(xremain[0]);
				break;
			}
		} else {
			sub1 = sub1_a;
			sub2 = xremain;
			if (xremain.size() == 0) {
				nongroups.push_back(sub1);
				break;
			}
		}
	}
	delete[] x;
	return make_tuple(seps, nongroups);
}

vector<size_t> RDGv2::interact(double *a, double af, vector<size_t> sub1, vector<size_t> sub2, vector<size_t> &xremain, BoundedObjectiveFunction<double>& fitf) {
	auto b = new double[fitf.dim()], c = new double[fitf.dim()], d = new double[fitf.dim()];
	for (int i = 0; i < fitf.dim(); i++) b[i] = c[i] = a[i];
	for (int i = 0; i < sub1.size(); i++) b[sub1[i]] = fitf.x_bound_max(sub1[i]);
	auto bf = fitf(b);
	auto delta1 = af - bf;
	for (int i = 0; i < fitf.dim(); i++) d[i] = b[i];
	for (int i = 0; i < sub2.size(); i++) c[sub2[i]] = d[sub2[i]] = (fitf.x_bound_max(sub2[i]) - fitf.x_bound_min(sub2[i])) / 2;
	auto cf = fitf(c), df = fitf(d);
	auto delta2 = cf - df;
	delete[] b, delete[] c, delete[] d;
	double epsilon = gamma(pow(fitf.dim(), 0.5) + 2) * (abs(af) + abs(bf) + abs(cf) + abs(df));
	if (abs(delta1 - delta2) > epsilon) {
		if (sub2.size() == 1) {
			sub1 = vunion(sub1, sub2);
		} else {
			unsigned int k = floor(sub2.size() / 2);
			auto sub2_1 = vector<size_t>();
			for (int i = 0; i < k; i++) sub2_1.push_back(sub2[i]);
			auto sub2_2 = vector<size_t>();
			for (int i = 0; i < sub2.size() - k; i++) sub2_2.push_back(sub2[k + i]);
			auto sub1_1 = interact(a, af, sub1, sub2_1, xremain, fitf);
			auto sub1_2 = interact(a, af, sub1, sub2_2, xremain, fitf);
			sub1 = vunion(sub1_1, sub1_2);
		}
	} else {
		xremain.insert(xremain.end(), sub2.begin(), sub2.end());
	}
	return sub1;
}

double RDGv2::gamma(double n) {
	double mu = n * (eps / 2);
	return mu / (1 - mu);
}
