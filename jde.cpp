#include"jde.hpp"

jDE::jDE() : DE() {}

jDE::jDE(size_t no_thr, size_t seed) : DE(no_thr, seed) {}

jDE::jDE(size_t no_thr) : DE(no_thr) {}

jDE::~jDE() {
	Fs.clear();
	CRs.clear();
}

string jDE::info() {
	return " Brest, J., Greiner, S., Boskovic, B., Mernik, M. and Zumer, V. (2006) Self-Adapting Control Parameters in Differential Evolution: A Comparative Study on Numerical Benchmark Problems. IEEE Transactions on Evolutionary Computation, 10, 646-657. https://doi.org/10.1109/TEVC.2006.872133 ";
}

string jDE::sinfo() {
	string r = "jDE";
	if (no_thr > 1) r = r + 'p';
	return r;
}

void jDE::setParameters(AlgParams *params) {
	DE::setParameters(params);
	this->tao_1 = getParam(params, "tao_1", 0.1);
	this->tao_2 = getParam(params, "tao_2", 0.1);
	this->F_s = getParam(params, "F_s", 0.5);
	this->CR_s = getParam(params, "CR_s", 0.9);
}

void jDE::initRun(TestFuncBounds<double> *func) {
	DE::initRun(func);
	for (int i = 0; i < np; i++) {
		Fs.push_back(F_s);
		CRs.push_back(CR_s);
	}
}

tuple<double, vector<double>> jDE::run(TestFuncBounds<double> *func) {
	initRun(func);
	auto r = DE::run(func);
	for (auto e : pop) delete [] e;
	Fs.clear();
	CRs.clear();
	return r;
}

void jDE::run_iteration() {
	auto s = ceil(np / double(no_thr));
	auto y = new double[fitf->dim];
	for (int i = s * optcpp::tid; i < s * (optcpp::tid + 1); i++) if (i < np) {
		double F_n = F_l + rand() * F_u;
		Fs[i] = (rand() < tao_1) ? F_n : Fs[i];
		double CR_n = rand();
		CRs[i] = (rand() < tao_2) ? CR_n : CRs[i];
		auto f = opt(*this, i, y);
		sync->arrive_and_wait();
		if (f < popf[i]) {
			for (int j = 0; j < fitf->dim; j++) pop[i][j] = y[j];
			popf[i] = f;
			setBestSolution(y, f);
		}
		sync->arrive_and_wait();
	} else {
		sync->arrive_and_wait();
		sync->arrive_and_wait();
	}
	delete [] y;
}

double jDE::rand_1(int i, double* y) {
	size_t a, b, c;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	int r = rand() % fitf->dim;
	for (int j = 0; j < fitf->dim; j++) {
		if (randDouble() < CRs[i] || j == r) {
			y[j] = pop[a][j] + Fs[i] * (pop[b][j] - pop[c][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return eval(y);
}

double jDE::best_2(int i, double *y) {
	int a, b, c, d;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	do d = rand() % np; while (d == i || d == b || d == a || d == c);
	int r = rand() % fitf->dim;
	for (int j = 0; j < fitf->dim; j++) {
		if (randDouble() < CRs[i] || j == r) {
			y[j] = x_best[j] + Fs[i] * (pop[a][j] - pop[b][j]) + F * (pop[c][j] - pop[d][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return eval(y);
}

double jDE::rand_to_best_1(int i, double *y) {
	int a, b, c;
	do a = rand() % np; while (a == i);
	do b = rand() % np; while (b == i || b == a);
	do c = rand() % np; while (c == i || c == b || c == a);
	int r = rand() % fitf->dim;
	for (int j = 0; j < fitf->dim; j++) {
		if (randDouble() < CRs[i] || j == r) {
			y[j] = pop[a][j] + Fs[i] * (x_best[j] - pop[i][j]) + F * (pop[b][j] - pop[c][j]);
		} else {
			y[j] = pop[i][j];
		}
	}
	fix_solution(*this, y);
	return eval(y);
}
