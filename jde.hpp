#ifndef _JDE_H_
#define _JDE_H_

#include "de.hpp"

using namespace std;

class jDE: public DE {
public:
    double F_l = 0.1;
    double F_u = 0.9;
    double tao_1;
    double tao_2;
    double F_s;
    double CR_s;

    jDE() : DE() {
        tao_1 = 0.1;
        tao_2 = 0.1;
        F_s = 0.45;
        CR_s = rand(0);
    };

    jDE(size_t np, double tao_1, double tao_2, size_t no_thr, size_t seed) : DE(np, 0, 0, no_thr, seed) {
        this->tao_1 = tao_1;
        this->tao_2 = tao_2;
    };

    jDE(size_t np, double tao_1, double tao_2, size_t no_thr) : DE(np, 0, 0, no_thr) {
        this->tao_1 = tao_1;
        this->tao_2 = tao_2;
    };

    ~jDE() {
        Fs.clear();
        CRs.clear();
    };

    virtual string info() override;
    virtual string sinfo() override;
    void initRun(TestFuncBounds*) override;
    virtual tuple<double, vector<double>> run(TestFuncBounds*) override;
    virtual void run_iteration(int) override;

    virtual double rand_1(int, int, double*) override;
    virtual double best_2(int, int, double*) override;
    virtual double rand_to_best_1(int, int, double*) override;

protected:
    vector<double> Fs;
    vector<double> CRs;
    std::function<double(jDE&, int, int, double*)> opt = &jDE::rand_1;

};

#endif
