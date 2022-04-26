#ifndef _JSOP_H_
#define _JSOP_H_

#include <vector>
#include <cmath>
#include <limits>
#include <thread>
#include <barrier>
#include <atomic>

#include "jso.hpp"

using namespace std;

class jSOp: public jSO {
private:
    vector<double*> pop;
    vector<double>  fitness;
    vector<double*> children;
    vector<double>  children_fitness;

    vector<double*> archive;
    vector<double>  success_sf;
    vector<double>  success_cr;
    vector<double>  dif_fitness;
    vector<double>  memory_sf;
    vector<double>  memory_cr;

    double* pop_sf = nullptr;
    double* pop_cr = nullptr;
    int* sorted_array = nullptr;
    double* temp_fit = nullptr;

    int p_num;
    int arc_size;
    int memory_pos;
    int max_pop_size;
    int min_pop_size = 4;
    int plan_pop_size;
    std::atomic<int> arc_ind_count;
    int num_success_params = 0;
    int old_num_success_params = 0;

    int no_thr;
    std::barrier<>* sync_threads;

public:
    jSOp(int g_pop_size, double g_arc_rate, double g_p_best_rate, int g_memory_size, int ino_thr) : jSO(g_pop_size, g_arc_rate, g_p_best_rate, g_memory_size) {
        no_thr = ino_thr;
    }

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds*);
    void initRun(TestFuncBounds*);
    void clean();
    void run_ind(int id);
};

#endif
