#ifndef _JSO_H_
#define _JSO_H_

#include <vector>
#include <cmath>
#include <limits>

#include "search_algorithm.hpp"

using namespace std;

class jSO: public SearchAlgorithm<double> {
public:
	jSO();
	~jSO();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<double, vector<double>> run(BoundedObjectiveFunction<double>*) override;
	virtual void run_iteration() override;
	void modifySolutionWithParentMedium(double*, double*);
	void evaluatePopulation(vector<double*>&, vector<double>&);
	void setSHADEParameters(double, double, int);
	void reducePopulationWithSort(vector<double*>&, vector<double>&);
	void operateCurrentToPBest1BinWithArchive(const vector<double*>&, double*, int&, int&, double&, double&, const vector<double*>&, int);

protected:
	/**
	 * http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Cauchy
	 * @brief cauchy_g Return random value from Cauchy distribution with mean "mu" and variance "gamma"
	 * @return Real number
	 */
	double cauchy_g(double, double);
	/**
	 * http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html#Gauss
	 * @brief gauss Return random value from normal distribution with mean "mu" and variance "gamma"
	 * @return Real number
	 */
	double gauss(double, double);

public:
	double epsilon;
	double arc_rate;
	double p_best_rate;
	size_t arc_size;
	size_t np;
	size_t memory_size;
	size_t reduction_ind_num;
};

#endif
