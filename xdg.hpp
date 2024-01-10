#ifndef _XDG_H_
#define _XDG_H_

#include "algorithm.hpp"

#include <algorithm>

// -------------------- XDG algorithm --------------------

class XDG: public AnalAlgorithm<double> {
public:
	XDG();
	XDG(const XDG&);
	~XDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>&) override;

protected:
	/**
	 * @brief epsilon
	 * @param y1
	 * @param y2
	 * @param y3
	 * @param y4
	 * @param dim
	 * @return
	 */
	virtual double epsilon(double, double, double, double, size_t);

	/**
	 * @brief _epsilon
	 */
	double _epsilon = 10e-12;
};

#endif
