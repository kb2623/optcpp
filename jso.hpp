#ifndef _JSO_H_
#define _JSO_H_

#include <vector>
#include <cmath>
#include <limits>

#include "search_algorithm.hpp"

using namespace std;

class jSO: public SearchAlgorithm {
public:
	jSO();
	~jSO();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams*) override;
	virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
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

//Recursive quick sort with index array
template<class VarType>
void sortIndexWithQuickSort(VarType array[], int first, int last, int index[]) {
	VarType x = array[(first + last) / 2];
	int i = first;
	int j = last;
	VarType temp_var = 0;
	int temp_num = 0;
	while (true) {
		while (array[i] < x) i++;
		while (x < array[j]) j--;
		if (i >= j) break;
		temp_var = array[i];
		array[i] = array[j];
		array[j] = temp_var;
		temp_num = index[i];
		index[i] = index[j];
		index[j] = temp_num;
		i++;
		j--;
	}
	if (first < (i - 1)) sortIndexWithQuickSort(array, first, i - 1, index);
	if ((j + 1) < last) sortIndexWithQuickSort(array, j + 1, last, index);
}

#endif
