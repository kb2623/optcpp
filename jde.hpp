#ifndef _JDE_H_
#define _JDE_H_

#include "parallel_search_algorithm.hpp"

using namespace std;

class jDE: public ParallelSearchAlgorithm {
public:
    size_t np;
    double F_l = 0.1;
    double F_u = 0.9;
    double tao_1;
    double tao_2;
    double F_s;
    double CR_s;

    jDE() : ParallelSearchAlgorithm() {
        np = 50;
        tao_1 = 0.1;
        tao_2 = 0.1;
        F_s = 0.45;
        CR_s = rand(0);
    };

    jDE(size_t np, double tao_1, double tao_2, size_t no_thr, size_t seed) : ParallelSearchAlgorithm(no_thr, seed) {
        this->np = np;
        this->tao_1 = tao_1;
        this->tao_2 = tao_2;
    };

    jDE(size_t np, double tao_1, double tao_2, size_t no_thr) : ParallelSearchAlgorithm(no_thr) {
        this->np = np;
        this->tao_1 = tao_1;
        this->tao_2 = tao_2;
    };

    ~jDE() {
        pop.clear();
        popf.clear();
        Fs.clear();
        CRs.clear();
    };

    virtual string info() override;
    virtual string sinfo() override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_thread(int) override;
    void initRun(TestFuncBounds*) override;

protected:
    vector<double*> pop;
    vector<double> popf;
    vector<double> Fs;
    vector<double> CRs;

};

#endif
