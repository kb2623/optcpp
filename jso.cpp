#include"jso.hpp"

jSO::jSO() {
    // TODO implementations
}

jSO::jSO(int g_pop_size, double g_arc_rate, double g_p_best_rate, int g_memory_size) {
    pop_size = g_pop_size;
    epsilon = pow(10.0, -8);
    setSHADEParameters(g_arc_rate, g_p_best_rate, g_memory_size);
}

jSO::~jSO() {}

string jSO::info() {
    return "Janez Brest, Mirjam Sepesy Maucec, Borko Boskovic. Single Objective Real-Parameter Optimization: Algorithm jSO, Proc. IEEE Congress on Evolutionary Computation (CEC-2017), Donostia - San Sebastián, Spain, June 2017.";
}

string jSO::sinfo() {
    return "jSO";
}

void jSO::setParameters(AlgParams &params) {
    // TODO
}

void jSO::evaluatePopulation(vector<double*> &pop, vector<double> &fitness) {
    for (int i = 0; i < pop_size; i++) {
        fitness[i] = eval(pop[i]);
        if (nfes >= func->max_num_evaluations) break;
    }
}

/*
  For each dimension j, if the mutant vector element v_j is outside the boundaries [x_min , x_max], we applied this bound handling method
  If you'd like to know that precisely, please read:
  J. Zhang and A. C. Sanderson, "JADE: Adaptive differential evolution with optional external archive,"
  IEEE Tran. Evol. Comput., vol. 13, no. 5, pp. 945–958, 2009.
 */
void jSO::modifySolutionWithParentMedium(double* child, double* parent) {
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

void jSO::run_iteration() {}

tuple<double, vector<double>> jSO::run(TestFuncBounds* func) {
    initRun(func);
    double p_best_rate_l = p_best_rate;
    vector <double*> pop(pop_size);
    vector <double> fitness(pop_size);
    vector <double*> children(pop_size);
    vector <double> children_fitness(pop_size, std::numeric_limits<double>::max());
    //initialize population
    for (int i = 0; i < pop_size; i++) {
        pop[i] = makeNewIndividual();
        fitness[i] = eval(pop[i]);
        children[i] = new double[func->dim];
    }
    //for external archive
    int arc_ind_count = 0, random_selected_arc_ind;
    vector <double*> archive;
    for (int i = 0; i < arc_size; i++) archive.push_back(new double[func->dim]);
    int num_success_params = 0, old_num_success_params = 0;
    vector <double> success_sf;
    vector <double> success_cr;
    vector <double> dif_fitness;
    // the contents of M_f and M_cr are all initialiezed 0.5
    vector <double> memory_sf(memory_size, 0.3);                // jSO
    vector <double> memory_cr(memory_size, 0.8);
    double temp_sum_sf, temp_sum_cr, sum, weight;
    //memory index counter
    int memory_pos = 0;
    //for new parameters sampling
    double mu_sf, mu_cr;
    int random_selected_period;
    double* pop_sf = new double[pop_size];
    double* pop_cr = new double[pop_size];
    //for current-to-pbest/1
    int p_best_ind, p_num = round(pop_size *  p_best_rate_l);
    int* sorted_array = new int[pop_size];
    double* temp_fit = new double[pop_size];
    // for linear population size reduction
    int min_pop_size = 4, plan_pop_size;
    //main loop
    while (!stop_cond(*this)) {
        for (int i = 0; i < pop.size(); i++) sorted_array[i] = i;
        for (int i = 0; i < pop.size(); i++) temp_fit[i] = fitness[i];
        sortIndexWithQuickSort(&temp_fit[0], 0, pop.size() - 1, sorted_array);
        for (int target = 0; target < pop.size(); target++) {
            //In each generation, CR_i and F_i used by each individual x_i are generated by first selecting an index r_i randomly from [1, H]
            random_selected_period = rand() % memory_size;
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
            if (p_num == 0) p_num = ceil(pop.size() *  p_best_rate_l) + 1;
            do {
                auto ind = rand() % p_num;
                p_best_ind = sorted_array[ind];
            } while (nfes < 0.50 * func->max_num_evaluations && p_best_ind == target);                   // iL-SHADE
            operateCurrentToPBest1BinWithArchive(pop, &children[target][0], target, p_best_ind, pop_sf[target], pop_cr[target], archive, arc_ind_count);
        }
        // evaluate the children's fitness values
        evaluatePopulation(children, children_fitness);
        //generation alternation
        for (int i = 0; i < pop.size(); i++) {
            if (children_fitness[i] == fitness[i]) {
                fitness[i] = children_fitness[i];
                for (int j = 0; j < func->dim; j ++) pop[i][j] = children[i][j];
            } else if (children_fitness[i] < fitness[i]) {
                setBestSolution(children[i], children_fitness[i]);
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
        old_num_success_params = num_success_params;
        num_success_params = success_sf.size();
        // if numeber of successful parameters > 0, historical memories are updated
        if (num_success_params > 0) {
            double old_sf, old_cr;     // Janez
            old_sf = memory_sf[memory_pos];
            old_cr = memory_cr[memory_pos];
            memory_sf[memory_pos] = 0;
            memory_cr[memory_pos] = 0;
            temp_sum_sf = 0;
            temp_sum_cr = 0;
            sum = 0;
            for (int i = 0; i < num_success_params; i++) sum += dif_fitness[i];
            //weighted lehmer mean
            for (int i = 0; i < num_success_params; i++) {
                weight = dif_fitness[i] / sum;
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
        plan_pop_size = round((((min_pop_size - pop_size) / double(func->max_num_evaluations)) * nfes) + pop_size);
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
    // Clean after run
    delete[] pop_sf;
    delete[] pop_cr;
    for (int i = 0; i < pop_size; i++) {
        delete[] pop[i]; //.clear();  // JANEZ
        delete[] children[i]; //.clear();  // JANEZ
    }
    pop.clear();  // JANEZ
    children.clear();  // JANEZ
    fitness.clear();
    children_fitness.clear();
    for (int i = 0; i < arc_size; i++) delete[] archive[i];
    archive.clear();
    memory_sf.clear();
    delete[] sorted_array;
    delete[] temp_fit; //.clear();
    return std::make_tuple(f_best, x_best);
}

void jSO::operateCurrentToPBest1BinWithArchive(const vector<double*> &pop, double* child, int &target, int &p_best_individual, double &scaling_factor, double &cross_rate, const vector<double*> &archive, int arc_ind_count) {
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
    modifySolutionWithParentMedium(child,  pop[target]);
}

void jSO::reducePopulationWithSort(vector<double*> &pop, vector<double> &fitness) {
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

void  jSO::setSHADEParameters(double g_arc_rate, double g_p_best_rate, int g_memory_size) {
    arc_rate = g_arc_rate;
    arc_size = (int)round(pop_size * arc_rate);
    p_best_rate = g_p_best_rate;
    memory_size = g_memory_size;
}
