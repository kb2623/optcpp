#ifndef _GDG_H_
#define _GDG_H_

#include "analize_algorithm.hpp"

class GDG: public AnalizeAlgorithm {
public:
	GDG();
	~GDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;

protected:
	tuple<vector<int>, vector<int>> graph_connected_components(vector<vector<double>>&, double);
	double calc_treshold(vector<vector<double>>&);
	vector<size_t> find_tresh(vector<double>&, double);

	size_t np;
	double epsilon;
	double temp;
};

#endif
