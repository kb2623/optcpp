#ifndef _XDG_H_
#define _XDG_H_

#include "analize_algorithm.hpp"
#include "objective_function.hpp"

#include <algorithm>

// -------------------- XDG algorithm params --------------------

class XDGParams : public RunAlgParams<double> {
public:
	XDGParams();
	XDGParams(BoundedObjectiveFunction<double>& fitf);
	~XDGParams();
};

// -------------------- XDG algorithm --------------------

class XDG: public AnalizeAlgorithm<double> {
public:
	XDG();
	~XDG();

	virtual string info() override;
	virtual string sinfo() override;
	virtual void setParameters(AlgParams& params) override;
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<double>& x) override;

protected:
	/**
	 * @brief initRun
	 * @param tdata
	 * @param fitf
	 * @return
	 */
	virtual RunAlgParams<double>& initRun(thread_data& tdata, BoundedObjectiveFunction<double>& fitf) override;
	/**
	 * @brief epsilon
	 * @param y1
	 * @param y2
	 * @param y3
	 * @param y4
	 * @return
	 */
	virtual double epsilon(double y1, double y2, double y3, double y4);

	/**
	 * @brief _epsilon
	 */
	double _epsilon;
};

#endif
