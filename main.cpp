#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include "cec17_test_func.hpp"
#include "sphere.hpp"

#include "xdg.hpp"
#include "xdgv1.hpp"
#include "xdgv2.hpp"
#include "rdg.hpp"
#include "rdgv2.hpp"
#include "rdgv3.hpp"
#include "ddg.hpp"
#include "ddgv1.hpp"
#include "ddgv2.hpp"
#include "dg.hpp"
#include "gdg.hpp"
#include "gdgv1.hpp"
#include "gdgv2.hpp"
#include "fii.hpp"

using std::cout, std::endl;


void runAnal(AnalAlgorithm<double>& algo, BoundedObjectiveFunction<double>& func) {
	auto ret = algo.run(func);
	auto gs = get<0>(ret);
	cout << "------------------------Seps-------------------------" << endl;
	cout << '(' << std::setw(3) << gs.size() << "): ";
	for (auto e : gs) cout << e << ' ';
	cout << endl;
	auto ngs = get<1>(ret);
	cout << "-----------------------NonSeps-----------------------" << endl;
	for (int k = 0; k < ngs.size(); k++) {
		cout << 'G' << std::setw(2) << k << " (" << std::setw(3) << ngs[k].size() << "): ";
		for (auto e: ngs[k]) cout << e << ' ';
		cout << endl;
	}
	cout << "-----------------------------------------------------" << endl;
}

void runCEC(vector<AnalAlgorithm<double>*>& algo) {
	int num_runs = 1;
	vector<int> problem_size = {10, 20, 30, 50, 100};
	for (auto it : problem_size) {
		auto no_evals = it * 10000;
		for (int i = 0; i < 30; i++) try {
			auto func = CEC17(it, i);
			for (auto a : algo) for (int j = 0; j < num_runs; j++) {
				cout << "Function = " << std::setw(2) << i + 1 << ", Dimension size = " << std::setw(3) << it << " > " << "Algorithm " << a->info() << endl;
				runAnal(*a, func);
				cout << endl;
			}
		} catch (const char* msg) {
			cout << msg << endl;
		}
	}
}

void runSphere(vector<AnalAlgorithm<double>*>& algs) {
	size_t no_runs = 1;
	size_t g_problem_size = 10;
	size_t g_max_num_evaluations = g_problem_size * 10000;
	for (auto a : algs) for (int i = 0; i < no_runs; i++) {
		auto func = Sphere(g_problem_size, g_max_num_evaluations);
		cout << "Dimension size = " << std::setw(3) << g_problem_size << ", run: " << i << " > " << a->info() << endl;
		runAnal(*a, func);
		cout << endl;
	}
	cout << endl;
}

int main() {
	vector<AnalAlgorithm<double>*> anals = {
		new XDG(),
		new XDGv1(),
		new XDGv2(),
		new RDG(),
		new RDGv2(),
		new RDGv3(),
		new DG(),
		new DDG(),
		new DDGv1(),
		new DDGv2(),
		new GDG(),
		new GDGv1(),
		new GDGv2(),
		new FII(),
	};
	runCEC(anals);
	//runSphere(anals);
	return 0;
}
