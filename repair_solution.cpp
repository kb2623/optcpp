#include "repair_solution.hpp"

template<typename T>
RepairSolution<T>::RepairSolution() : fix_solution_arr(&RepairSolution::fix_lim), fix_solution_vec(&RepairSolution::fix_lim) {}

template<typename T>
RepairSolution<T>::~RepairSolution() {}

template<typename T>
void RepairSolution<T>::set_fix_solution_arr(std::function<T*(RepairSolution&, T*)> sol_fix) {
	fix_solution_arr = sol_fix;
}

template<typename T>
void RepairSolution<T>::set_fix_solution_vec(std::function<vector<T>(RepairSolution&, vector<T>)> sol_fix) {
	fix_solution_vec = sol_fix;
}
