#include"jsop.hpp"

jSOp::jSOp() : ParallelSearchAlgorithm() {}

jSOp::jSOp(size_t no_thr) : ParallelSearchAlgorithm(no_thr) {}

jSOp::jSOp(size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed) {}

jSOp::~jSOp() {}

string jSOp::info() {
	return "jSO parallel";
}

string jSOp::sinfo() {
	return "jSOp";
}

void jSOp::setParameters(AlgParams *params) {
	SearchAlgorithm::setParameters(params);
	this->epsilon = getParam(params, "epsilon", pow(10.0, -8));
	this->np = getParam(params, "np", 50);
	setSHADEParameters(getParam(params, "arc_rate", .5), getParam(params, "p_best_rate", .5), getParam(params, "memory_size", 150));
}

/*
  For each dimension j, if the mutant vector element v_j is outside the boundaries [x_min , x_max], we applied this bound handling method
  If you'd like to know that precisely, please read:
  J. Zhang and A. C. Sanderson, "JADE: Adaptive differential evolution with optional external archive,"
  IEEE Tran. Evol. Comput., vol. 13, no. 5, pp. 945–958, 2009.
 */
void jSOp::modifySolutionWithParentMedium(double* child, const double* parent) {
	for (int j = 0; j < func->dim; j++) {
		if (child[j] < func->x_bound_min[j]) child[j]= (func->x_bound_min[j] + parent[j]) / 2.0;
		if (child[j] > func->x_bound_max[j]) child[j]= (func->x_bound_max[j] + parent[j]) / 2.0;
	}
}

void jSOp::operateCurrentToPBest1BinWithArchive(int id, vector<double*> pop, double* child, int &target, int &p_best_individual, double &scaling_factor, double &cross_rate, vector<double*> archive, int arc_ind_count) {
	int r1, r2;
	double jF = scaling_factor;                                // jSO
	if (nfes < 0.2 * func->max_num_evaluations) {
		jF = jF * 0.7;        // jSO
	} else if (nfes < 0.4 * func->max_num_evaluations) {
		jF = jF * 0.8;        // jSO
	} else {
		jF = jF * 1.2;      // jSO
	}
	do {
		r1 = rand(id) % pop.size();
	} while (r1 == target);
	do {
		r2 = rand(id) % (pop.size() + arc_ind_count);
	} while ((r2 == target) || (r2 == r1));
	int random_variable = rand(id) % func->dim;
	if (r2 >= pop.size()) {
		r2 -= pop.size();
		for (int i = 0; i < func->dim; i++) {
			if ((randDouble(id) < cross_rate) || (i == random_variable)) child[i] = pop[target][i] + jF * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - archive[r2][i]);  // jSO
			else child[i] = pop[target][i];
		}
	} else {
		for (int i = 0; i < func->dim; i++) {
			if ((randDouble(id) < cross_rate) || (i == random_variable)) child[i] = pop[target][i] + jF * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - pop[r2][i]);     // jSO
			else child[i] = pop[target][i];
		}
	}
	//If the mutant vector violates bounds, the bound handling method is applied
	modifySolutionWithParentMedium(child,  pop[target]);
}

void jSOp::reducePopulationWithSort(vector<double*> &pop, vector<double> &fitness) {
	int worst_ind;
	for (int i = 0; i < reduction_ind_num; i++) {
		worst_ind = 0;
		for (int j = 1; j < pop.size(); j++) {
			if (fitness[j] > fitness[worst_ind]) worst_ind = j;
		}
		pop.erase(pop.begin() + worst_ind);
		fitness.erase(fitness.begin() + worst_ind);
	}
}

void  jSOp::setSHADEParameters(double g_arc_rate, double g_p_best_rate, int g_memory_size) {
	arc_rate = g_arc_rate;
	arc_size = size_t(round(np * arc_rate));
	p_best_rate = g_p_best_rate;
	memory_size = g_memory_size;
}

tuple<double, vector<double>> jSOp::run(TestFuncBounds* ifun) {
	initRun(ifun);
	auto r = ParallelSearchAlgorithm::run(ifun);
	clean();
	return r;
}

void jSOp::initRun(TestFuncBounds *ifunc) {
	ParallelSearchAlgorithm::initRun(ifunc);
	pop = std::vector<double*>(), children = vector<double*>();
	fitness = vector<double>(), children_fitness = vector<double>(np, 0);
	for (int i = 0; i < np; i++) {
		pop.push_back(makeNewIndividual());
		fitness.push_back(eval(pop[i]));
		children.push_back(new double[func->dim]);
	}
	archive = vector<double*>();
	for (int i = 0; i < arc_size; i++) archive.push_back(new double[func->dim]);
	success_sf = vector<double>(), success_cr = vector<double>(), dif_fitness = vector<double>();
	memory_sf = std::vector<double>(memory_size, 0.3), memory_cr = vector<double>(memory_size, 0.8);
	pop_sf = new double[np], pop_cr = new double[np];
	sorted_array = new int[np];
	temp_fit = new double[np];
	p_num = round(np * p_best_rate);
	arc_ind_count = 0;
	num_success_params = 0;
	old_num_success_params = 0;
}

void jSOp::run_thread(int id) {
	auto s = ceil(np / double(no_thr));
	int p_best_ind, random_selected_arc_ind;
	double mu_sf, mu_cr, p_best_rate_l = p_best_rate;
	while (!stop_cond(*this)) {
		for (int i = s * id; i < pop.size() && i < s * (id + 1); i++) sorted_array[i] = i;
		for (int i = s * id; i < pop.size() && i < s * (id + 1); i++) temp_fit[i] = fitness[i];
		sync->wait();
		if (id == 0) sortIndexWithQuickSort(temp_fit, 0, pop.size() - 1, sorted_array);
		sync->wait();
		for (int target = s * id; target < pop.size() && target < s * (id + 1); target++) {
			//In each generation, CR_i and F_i used by each individual x_i are generated by first selecting an index r_i randomly from [1, H]
			size_t random_selected_period = rand(id) % memory_size;
			if(random_selected_period == memory_size - 1) {
				mu_sf = 0.9, mu_cr = 0.9;
			} else {
				mu_sf = memory_sf[random_selected_period];
				mu_cr = memory_cr[random_selected_period];
			}
			//generate CR_i and repair its value
			if (mu_cr < 0) {            // JANEZ
				pop_cr[target] = 0.0;     // LSHADE 0
			} else {
				pop_cr[target] = gauss(id, mu_cr, 0.1);
				if (pop_cr[target] > 1) pop_cr[target] = 1;
				else if (pop_cr[target] < 0) pop_cr[target] = 0;
			}
			if (nfes < 0.25 * func->max_num_evaluations && pop_cr[target] < 0.7) pop_cr[target] = 0.7;    // jSO
			if (nfes < 0.50 * func->max_num_evaluations && pop_cr[target] < 0.6) pop_cr[target] = 0.6;    // jSO
			//generate F_i and repair its value
			do {
				pop_sf[target] = cauchy_g(id, mu_sf, 0.1);
			} while (pop_sf[target] <= 0.0);
			if (pop_sf[target] > 1) pop_sf[target] = 1.0;
			if (nfes< 0.6 * func->max_num_evaluations && pop_sf[target] > 0.7) pop_sf[target] = 0.7;    // jSO
			//p-best individual is randomly selected from the top pop_size *  p_i members
			if (p_num == 0) p_num = ceil(pop.size() * p_best_rate_l) + 1;
			do {
				auto ind = p_num == 0 ? 0 : rand(id) % p_num;
				p_best_ind = sorted_array[ind];
			} while (nfes < 0.50 * func->max_num_evaluations && p_best_ind == target);                   // iL-SHADE
			operateCurrentToPBest1BinWithArchive(id, pop, children[target], target, p_best_ind, pop_sf[target], pop_cr[target], archive, arc_ind_count);
			children_fitness[target] = eval(children[target]);
		}
		//generation alternation
		sync->wait();
		for (int i = s * id; i < pop.size() && i < s * (id + 1); i++) {
			if (children_fitness[i] == fitness[i]) {
				fitness[i] = children_fitness[i];
				for (int j = 0; j < func->dim; j ++) pop[i][j] = children[i][j];
			} else if (children_fitness[i] < fitness[i]) {
				setBestSolution(children[i], children_fitness[i]);
				fitness[i] = children_fitness[i];
				success_lock.lock();
				dif_fitness.push_back(fabs(fitness[i] - children_fitness[i]));
				//successful parameters are preserved in S_F and S_CR
				success_sf.push_back(pop_sf[i]);
				success_cr.push_back(pop_cr[i]);
				success_lock.unlock();
				//parent vectors x_i which were worse than the trial vectors u_i are preserved
				if (arc_size > 1) {
					archive_lock.lock();
					if (arc_ind_count < arc_size) {
						for (int j = 0; j < func->dim; j++) archive[arc_ind_count][j] = pop[i][j];
						arc_ind_count++;
					} else { //Whenever the size of the archive exceeds, randomly selected elements are deleted to make space for the newly inserted elements
						random_selected_arc_ind = rand(id) % arc_size;
						for (int j = 0; j < func->dim; j++) archive[random_selected_arc_ind][j] = pop[i][j];
					}
					archive_lock.unlock();
				}
				for (int j = 0; j < func->dim; j ++) pop[i][j] = children[i][j];    // jSO
			}
		}
		sync->wait();
		if (id == 0) {
			old_num_success_params = num_success_params;
			num_success_params = success_sf.size();
			// if numeber of successful parameters > 0, historical memories are updated
			if (num_success_params > 0) {
				double old_sf, old_cr;     // Janez
				old_sf = memory_sf[memory_pos];
				old_cr = memory_cr[memory_pos];
				memory_sf[memory_pos] = 0;
				memory_cr[memory_pos] = 0;
				double temp_sum_sf = 0;
				double temp_sum_cr = 0;
				double sum = 0;
				for (int i = 0; i < num_success_params; i++) sum += dif_fitness[i];
				//weighted lehmer mean
				for (int i = 0; i < num_success_params; i++) {
					double weight = dif_fitness[i] / sum;
					memory_sf[memory_pos] += weight * success_sf[i] * success_sf[i];
					temp_sum_sf += weight * success_sf[i];
					memory_cr[memory_pos] += weight * success_cr[i] * success_cr[i];
					temp_sum_cr += weight * success_cr[i];
				}
				memory_sf[memory_pos] /= temp_sum_sf;
				if (temp_sum_cr == 0 || memory_cr[memory_pos] == -1) memory_cr[memory_pos] = -1;
				else memory_cr[memory_pos] /= temp_sum_cr;
				//JANEZ
				memory_sf[memory_pos] = (memory_sf[memory_pos] + old_sf) / 2.0;
				memory_cr[memory_pos] = (memory_cr[memory_pos] + old_cr) / 2.0;
				//increment the counter
				memory_pos++;
				if (memory_pos >= memory_size) memory_pos = 0;
				//clear out the S_F, S_CR and delta fitness
				success_sf.clear();
				success_cr.clear();
				dif_fitness.clear();
			}
			// calculate the population size in the next generation
			size_t plan_pop_size = round((((min_pop_size - pop.size()) / double(func->max_num_evaluations)) * nfes) + pop.size());
			if (pop.size() > plan_pop_size) {
				reduction_ind_num = pop.size() - plan_pop_size;
				if (pop.size() - reduction_ind_num <  min_pop_size) reduction_ind_num = pop.size() - min_pop_size;
				reducePopulationWithSort(pop, fitness);
				// resize the archive size
				arc_size = pop.size() * arc_rate;
				if (arc_ind_count > arc_size) arc_ind_count = arc_size;
				// resize the number of p-best individuals
				p_best_rate_l = p_best_rate_l * (1.0 - 0.5 * nfes /  double(func->max_num_evaluations));   // JANEZ
				p_num = round(pop.size() *  p_best_rate_l);
				if (p_num <= 1)  p_num = 2;
			}
		}
		sync->wait();
	}
}

void jSOp::run_iteration(int id) {}

void jSOp::clean() {
	delete[] pop_sf;
	delete[] pop_cr;
	for (auto e : pop) delete [] e;
	pop.clear();  // JANEZ
	for (auto e : children) delete [] e;
	children.clear();  // JANEZ
	fitness.clear();
	children_fitness.clear();
	for (auto e : archive) delete [] e;
	archive.clear();
	memory_sf.clear();
	delete[] sorted_array;
	delete[] temp_fit; //.clear();
}
