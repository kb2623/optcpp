#ifndef _DDGV2_H_
#define _DDGV2_H_

#include "ddg.hpp"

class DDGv2: public DDG {
public:
	DDGv2();
	~DDGv2();

	virtual string info() override;
	virtual string sinfo() override;

protected:
	double get_epsilon_addi(double, double, double, double);
	double gamma(double);

	double eps = 10e-12;
};

#endif
