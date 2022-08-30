#include "stopping_condition.hpp"

StoppingCondition::StoppingCondition() : end_run(&StoppingCondition::no_limit) {}

StoppingCondition::~StoppingCondition() {}

bool StoppingCondition::run_time() {
	std::chrono::time_point<std::chrono::steady_clock> curr = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = curr - start_time;
	return elapsed_seconds.count() >= max_execution_time;
}

bool StoppingCondition::no_limit() {
	return false;
}

void StoppingCondition::set_end_run(std::function<bool(StoppingCondition&)> condition) {
	end_run = condition;
}

void StoppingCondition::start_timer() {
	start_time = std::chrono::steady_clock::now();
}

void StoppingCondition::set_max_no_fes(unsigned long long int lim_fes) {
	lim_no_fes = lim_fes;
}

void StoppingCondition::set_max_no_gen(unsigned long long int lim_gen) {
	lim_no_gen = lim_gen;
}

void StoppingCondition::set_fitness_target_value(double fitness) {
	fitness_target_value = fitness;
}
