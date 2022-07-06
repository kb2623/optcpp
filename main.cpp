#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include "cec17_test_func.hpp"
#include "sphere.hpp"

#include "example.hpp"
#include "de.hpp"
#include "jde.hpp"
#include "jso.hpp"
#include "jsop.hpp"

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

void runAnal(AnalizeAlgorithm *algo, TestFuncBounds *func) {
	algo->setParameters(nullptr);
	auto ret = algo->run(func);
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

void runAlgo(SearchAlgorithm *algo, TestFuncBounds *func) {
	cout << algo->sinfo() << " : ";
	auto r = algo->run(func);
	for (auto e : get<1>(r)) cout << e << ' ';
	auto val = algo->eval(get<1>(r).data());
	cout << ": ";
	cout << get<0>(r) << " : ";
	if (val == get<0>(r)) cout << "OK";
	else cout << "ERROR >> " << val;
	cout << " : " << algo->get_nfes() - 1;
}

void runCEC(vector<SearchAlgorithm*> &algs) {
	size_t no_runs = 10;
	size_t g_problem_size = 10;
	for (int i = 0; i < 30; i++) try {
		size_t g_max_num_evaluations = g_problem_size * 10000;
		for (auto a : algs) {
			a->setParameters(nullptr);
			auto func = CEC17(g_problem_size, i, g_max_num_evaluations);
			for (int j = 0; j < no_runs; j++) {
				cout << "F" << i + 1 << " : ";
				runAlgo(a, &func);
				cout << endl;
			}
		}
		cout << endl;
	} catch (const char* msg) {
		cerr << msg << endl;
	}
}

void runSphere(vector<SearchAlgorithm*> &algs) {
	size_t no_runs = 10;
	size_t g_problem_size = 10;
	size_t g_max_num_evaluations = g_problem_size * 10000;
	for (int i = 0; i < no_runs; i++) for (auto a : algs) {
		auto func = Sphere(g_problem_size, g_max_num_evaluations);
		runAlgo(a, &func);
		cout << endl;
	}
	cout << endl;
}

void runCEC(vector<AnalizeAlgorithm*> &algo) {
	int num_runs = 1;
	vector<int> problem_size = {10, 20, 30, 50, 100};
	for (auto it : problem_size) {
		auto no_evals = it * 10000;
		for (int i = 0; i < 30; i++) try {
			auto func = CEC17(it, i, no_evals);
			for (auto e : algo) for (int j = 0; j < num_runs; j++) {
				cout << "Function = " << std::setw(2) << i + 1 << ", Dimension size = " << std::setw(3) << it << " > " << "Algorithm " << e->info() << endl;
				runAnal(e, &func);
				cout << endl;
			}
		} catch (const char* msg) {
			cout << msg << endl;
		}
	}
}

void runSphere(vector<AnalizeAlgorithm*> &algs) {
	size_t no_runs = 1;
	size_t g_problem_size = 10;
	size_t g_max_num_evaluations = g_problem_size * 10000;
	for (auto a : algs) for (int i = 0; i < no_runs; i++) {
		auto func = Sphere(g_problem_size, g_max_num_evaluations);
		cout << "Dimension size = " << std::setw(3) << g_problem_size << ", run: " << i << " > " << a->info() << endl;
		runAnal(a, &func);
		cout << endl;
	}
	cout << endl;
}

int main() {
	vector<AnalizeAlgorithm*> anals = {
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
	//runCEC(anals);
	runSphere(anals);
	const auto no_workers = 10;
	/*
	vector<SearchAlgorithm*> algs = {
		new DE(),
		new DE(no_workers),
		new jDE(),
		new jDE(no_workers),
		new jSO(),
		new jSOp(no_workers),
		new Bar(no_workers),
	};
	runCEC(algs);
	//runSphere(algs);
	*/
	return 0;
}
