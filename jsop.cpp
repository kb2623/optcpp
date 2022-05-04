#include"jsop.hpp"

std::string jSOp::info() {
    return "jSO parallel";
}

void jSOp::evaluatePopulation(vector<vector<double>> &pop, vector<double> &fitness) {
    for (int i = 0; i < pop_size; i++) {
        fitness[i] = eval(pop[i].data());
        if (nfes >= func->max_num_evaluations) break;
    }
}

/*
  For each dimension j, if the mutant vector element v_j is outside the boundaries [x_min , x_max], we applied this bound handling method
  If you'd like to know that precisely, please read:
  J. Zhang and A. C. Sanderson, "JADE: Adaptive differential evolution with optional external archive,"
  IEEE Tran. Evol. Comput., vol. 13, no. 5, pp. 945–958, 2009.
 */
void jSOp::modifySolutionWithParentMedium(double* child, const double* parent) {
    int l_problem_size = func->dim;
    double* l_min_region = func->x_bound_min;
    double* l_max_region = func->x_bound_max;
    for (int j = 0; j < l_problem_size; j++) {
        if (child[j] < l_min_region[j]) {
            child[j]= (l_min_region[j] + parent[j]) / 2.0;
        } else if (child[j] > l_max_region[j]) {
            child[j]= (l_max_region[j] + parent[j]) / 2.0;
        }
    }
}

void jSOp::operateCurrentToPBest1BinWithArchive(const vector<vector<double>> &pop, double* child, int &target, int &p_best_individual, double &scaling_factor, double &cross_rate, const vector<vector<double>> &archive, int arc_ind_count) {
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
        r1 = rand() % pop_size;
    } while (r1 == target);
    do {
        r2 = rand() % (pop_size + arc_ind_count);
    } while ((r2 == target) || (r2 == r1));
    int random_variable = rand() % func->dim;
    if (r2 >= pop_size) {
        r2 -= pop_size;
        for (int i = 0; i < func->dim; i++) {
            if ((randDouble() < cross_rate) || (i == random_variable)) child[i] = pop[target][i] + jF * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - archive[r2][i]);  // jSO
            else child[i] = pop[target][i];
        }
    } else {
        for (int i = 0; i < func->dim; i++) {
            if ((randDouble() < cross_rate) || (i == random_variable)) child[i] = pop[target][i] + jF * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - pop[r2][i]);     // jSO
            else child[i] = pop[target][i];
        }
    }
    //If the mutant vector violates bounds, the bound handling method is applied
    modifySolutionWithParentMedium(child,  pop[target].data());
}

void jSOp::reducePopulationWithSort(vector<vector<double>> &pop, vector<double> &fitness) {
    int worst_ind;
    for (int i = 0; i < reduction_ind_num; i++) {
        worst_ind = 0;
        for (int j = 1; j < pop_size; j++) {
            if (fitness[j] > fitness[worst_ind]) worst_ind = j;
        }
        pop.erase(pop.begin() + worst_ind);
        fitness.erase(fitness.begin() + worst_ind);
        pop_size--;
    }
}

void  jSOp::setSHADEParameters(double g_arc_rate, double g_p_best_rate, int g_memory_size) {
    arc_rate = g_arc_rate;
    arc_size = size_t(round(pop_size * arc_rate));
    p_best_rate = g_p_best_rate;
    memory_size = g_memory_size;
}

std::tuple<double, std::vector<double>> jSOp::run(TestFuncBounds* ifun) {
    initRun(ifun);
    auto r = ParallelSearchAlgorithm::run(ifun);
    clean();
    return r;
}

void jSOp::initRun(TestFuncBounds *ifunc) {
    ParallelSearchAlgorithm::initRun(ifunc);
    pop = std::vector<std::vector<double>>(), children = std::vector<std::vector<double>>(pop_size);
    fitness = std::vector<double>(pop_size, 0), children_fitness = std::vector<double>(pop_size, 0);
    for (int i = 0; i < pop_size; i++) {
        auto v = std::vector<double>(func->dim);
        for (int j = 0; j < func->dim; j++) v[j] = ((func->x_bound_max[j] - func->x_bound_min[j]) * randDouble()) + func->x_bound_min[j];
        pop.push_back(v);
    }
    archive = std::vector<std::vector<double>>(arc_size, std::vector<double>(func->dim, 0));
    success_sf = std::vector<double>(), success_cr = std::vector<double>(), dif_fitness = std::vector<double>();
    memory_sf = std::vector<double>(memory_size, 0.3), memory_cr = vector<double>(memory_size, 0.8);
    pop_sf = new double[pop_size], pop_cr = new double[pop_size];
    sorted_array = new int[pop_size];
    temp_fit = new double[pop_size];
    max_pop_size = pop_size;
    p_num = round(pop_size * p_best_rate);
    arc_ind_count = 0;
    num_success_params = 0;
    old_num_success_params = 0;
}

void jSOp::run_thread(int id) {
    auto s = ceil(pop_size / double(no_thr));
    int p_best_ind, random_selected_arc_ind;
    double mu_sf, mu_cr;
    while (nfes < func->max_num_evaluations) {
        for (int i = s * id; i < pop_size && i < s * (id + 1); i++) sorted_array[i] = i;
        for (int i = s * id; i < pop_size && i < s * (id + 1); i++) temp_fit[i] = fitness[i];
        sync_point->arrive_and_wait();
        if (id == 0) sortIndexWithQuickSort(&temp_fit[0], 0, pop_size - 1, sorted_array);
        for (int target = s * id; target < pop_size && target < s * (id + 1); target++) {
            //In each generation, CR_i and F_i used by each individual x_i are generated by first selecting an index r_i randomly from [1, H]
            size_t random_selected_period = rand() % memory_size;
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
                pop_cr[target] = gauss(mu_cr, 0.1);
                if (pop_cr[target] > 1) pop_cr[target] = 1;
                else if (pop_cr[target] < 0) pop_cr[target] = 0;
            }
            if (nfes < 0.25 * func->max_num_evaluations && pop_cr[target] < 0.7) pop_cr[target] = 0.7;    // jSO
            if (nfes < 0.50 * func->max_num_evaluations && pop_cr[target] < 0.6) pop_cr[target] = 0.6;    // jSO
            //generate F_i and repair its value
            do {
                pop_sf[target] = cauchy_g(mu_sf, 0.1);
            } while (pop_sf[target] <= 0.0);
            if (pop_sf[target] > 1) pop_sf[target] = 1.0;
            if (nfes< 0.6 * func->max_num_evaluations && pop_sf[target] > 0.7) pop_sf[target] = 0.7;    // jSO
            //p-best individual is randomly selected from the top pop_size *  p_i members
            do {
                auto ind = p_num == 0 ? 0 : rand() % p_num;
                p_best_ind = sorted_array[ind];
            } while (nfes < 0.50 * func->max_num_evaluations && p_best_ind == target);                   // iL-SHADE
            operateCurrentToPBest1BinWithArchive(pop, &children[target][0], target, p_best_ind, pop_sf[target], pop_cr[target], archive, arc_ind_count);
            children_fitness[target] = eval(children[target].data());
        }
        sync_point->arrive_and_wait();
        //generation alternation
        for (int i = s * id; i < pop_size && i < s * (id + 1); i++) {
            if (children_fitness[i] == fitness[i]) {
                fitness[i] = children_fitness[i];
                for (int j = 0; j < func->dim; j ++) pop[i][j] = children[i][j];
            } else if (children_fitness[i] < fitness[i]) {
                dif_fitness.push_back(fabs(fitness[i] - children_fitness[i]));
                fitness[i] = children_fitness[i];
                //successful parameters are preserved in S_F and S_CR
                success_sf.push_back(pop_sf[i]);
                success_cr.push_back(pop_cr[i]);
                //parent vectors x_i which were worse than the trial vectors u_i are preserved
                if (arc_size > 1) {
                    if (arc_ind_count < arc_size) {
                        for (int j = 0; j < func->dim; j++) archive[arc_ind_count][j] = pop[i][j];
                        arc_ind_count++;
                    } else { //Whenever the size of the archive exceeds, randomly selected elements are deleted to make space for the newly inserted elements
                        random_selected_arc_ind = rand() % arc_size;
                        for (int j = 0; j < func->dim; j++) archive[random_selected_arc_ind][j] = pop[i][j];
                    }
                }
                for (int j = 0; j < func->dim; j ++) pop[i][j] = children[i][j];    // jSO
            }
        }
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
            size_t plan_pop_size = round((((min_pop_size - max_pop_size) / double(func->max_num_evaluations)) * nfes) + max_pop_size);
            if (pop_size > plan_pop_size) {
                reduction_ind_num = pop_size - plan_pop_size;
                if (pop_size - reduction_ind_num <  min_pop_size) reduction_ind_num = pop_size - min_pop_size;
                reducePopulationWithSort(pop, fitness);
                // resize the archive size
                arc_size = pop_size * arc_rate;
                if (arc_ind_count > arc_size) arc_ind_count = arc_size;
                // resize the number of p-best individuals
                p_best_rate = p_best_rate * (1.0 - 0.5 * nfes /  double(func->max_num_evaluations));   // JANEZ
                p_num = round(pop_size *  p_best_rate);
                if (p_num <= 1)  p_num = 2;
            }
        }
        sync_point->arrive_and_wait();
    }
}

void jSOp::clean() {
    delete[] pop_sf;
    delete[] pop_cr;
    pop.clear();  // JANEZ
    children.clear();  // JANEZ
    fitness.clear();
    children_fitness.clear();
    archive.clear();
    memory_sf.clear();
    delete[] sorted_array;
    delete[] temp_fit; //.clear();
}
