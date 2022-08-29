#ifndef _JDE_H_
#define _JDE_H_

#include "de.hpp"

using namespace std;

class jDE: public DE {
public:
	jDE();
	jDE(size_t);
	jDE(size_t, size_t);
	~jDE();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	void initRun(TestFuncBounds<double>*) override;
	virtual tuple<double, vector<double>> run(TestFuncBounds<double>*) override;
	virtual void run_iteration() override;

	virtual double rand_1(int, double*) override;
	virtual double best_2(int, double*) override;
	virtual double rand_to_best_1(int, double*) override;

protected:
	vector<double> Fs;
	vector<double> CRs;
	std::function<double(jDE&, int, double*)> opt = &jDE::rand_1;

	double F_l = 0.1;
	double F_u = 0.9;
	double tao_1;
	double tao_2;
	double F_s;
	double CR_s;
};

#endif
