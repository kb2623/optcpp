#ifndef _STOPPING_CONDITION_H_
#define _STOPPING_CONDITION_H_

class StoppingCondition {
public:
	StoppingCondition();
	~StoppingCondition();

	bool max_no_fes();
	bool max_no_gen();
	bool target_value();
	bool run_time();
};

#endif
