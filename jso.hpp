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

	double epsilon;
	double arc_rate;
	double p_best_rate;
	size_t arc_size;
	size_t np;
	size_t memory_size;
	size_t reduction_ind_num;
};

#endif
