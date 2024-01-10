#ifndef _GDG_H_
#define _GDG_H_

#include "algorithm.hpp"

class GDG: public AnalAlgorithm<double> {
public:
	GDG();
	GDG(const GDG&);
	~GDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	tuple<vector<int>, vector<int>> graph_connected_components(vector<vector<double>>&, double, BoundedObjectiveFunction<double>&);
	double calc_treshold(vector<vector<double>>&, BoundedObjectiveFunction<double>&);
	vector<size_t> find_tresh(vector<double>&, double);

	size_t np;
	double epsilon;
	double temp;
};

#endif
