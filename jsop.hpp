#ifndef _JSOP_H_
#define _JSOP_H_

#include "parallel_search_algorithm.hpp"
#include "jso.hpp"

using namespace std;

class jSOp: public ParallelSearchAlgorithm {
public:
    jSOp(int g_pop_size, double g_arc_rate, double g_p_best_rate, int g_memory_size, size_t no_thr) : ParallelSearchAlgorithm(no_thr) {
        pop_size = g_pop_size;
        epsilon = pow(10.0, -8);
        setSHADEParameters(g_arc_rate, g_p_best_rate, g_memory_size);
    }

    jSOp(int g_pop_size, double g_arc_rate, double g_p_best_rate, int g_memory_size) : jSOp(g_pop_size, g_arc_rate, g_p_best_rate, g_memory_size, 1) {}

    virtual std::string info() override;
    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds*) override;
    virtual void run_thread(int) override;
    virtual void initRun(TestFuncBounds*) override;
    void modifySolutionWithParentMedium(double*, const double*);
    void evaluatePopulation(vector<vector<double>>&, vector<double>&);
    void setSHADEParameters(double, double, int);
    void reducePopulationWithSort(vector<vector<double>>&, vector<double>&);
    void operateCurrentToPBest1BinWithArchive(const vector<vector<double>>&, double*, int&, int&, double&, double&, const vector<vector<double>>&, int);

private:
    void clean();

protected:
    double epsilon;
    double arc_rate;
    double p_best_rate;
    int reduction_ind_num;
    size_t pop_size;
    size_t arc_size;
    size_t memory_size;
    size_t memory_pos;
    size_t min_pop_size, max_pop_size, p_num, arc_ind_count, num_success_params, old_num_success_params;
    std::vector<std::vector<double>> pop;
    std::vector<double> fitness;
    std::vector<std::vector<double>> children;
    std::vector<double> children_fitness;
    std::vector<std::vector<double>> archive;
    std::vector<double> success_sf;
    std::vector<double> success_cr;
    std::vector<double> memory_sf;
    std::vector<double> memory_cr;
    std::vector<double> dif_fitness;
    double* pop_sf;
    double* pop_cr;
    double* temp_fit;
    int* sorted_array;
};

#endif
