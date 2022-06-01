#ifndef _DE_MUTATIONS_H_
#define _DE_MUTATIONS_H_

class DeMutations {
public:
	DeMutations() {};
	~DeMutations() {};

	virtual double rand_1(int, int, double*) = 0;
	virtual double best_2(int, int, double*) = 0;
	virtual double rand_to_best_1(int, int, double*) = 0;
};

#endif
