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

    virtual string info() override;
    virtual string sinfo() override;
    virtual void setParameters(AlgParams&) override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_iteration(int) override;
    virtual void initRun(TestFuncBounds*) override;
    virtual void run_thread(int) override;

protected:
    void modifySolutionWithParentMedium(double*, const double*);
    void setSHADEParameters(double, double, int);
    void reducePopulationWithSort(vector<double*>&, vector<double>&);
    void operateCurrentToPBest1BinWithArchive(int, vector<double*>, double*, int&, int&, double&, double&, vector<double*>, int);


private:
    std::mutex success_lock;
    std::mutex archive_lock;

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
    vector<double*> pop;
    vector<double> fitness;
    vector<double*> children;
    vector<double> children_fitness;
    vector<double*> archive;
    vector<double> success_sf;
    vector<double> success_cr;
    vector<double> memory_sf;
    vector<double> memory_cr;
    vector<double> dif_fitness;
    double* pop_sf;
    double* pop_cr;
    double* temp_fit;
    int* sorted_array;
};

#endif
