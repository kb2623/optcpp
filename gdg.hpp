#ifndef _GDG_H_
#define _GDG_H_

#include "analize_algorithm.hpp"

class GDG: public AnalizeAlgorithm {
public:
    size_t np;
    double epsilon;
    double temp;

    GDG() : GDG(10, 10e-10) {};
    GDG(size_t np, double epsilon) : np(np), epsilon(epsilon) {};
    ~GDG() {};

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    tuple<vector<int>, vector<int>> graph_connected_components(vector<vector<double>>&, double);
    double calc_treshold();
    vector<size_t> find_tresh(vector<double> &, double);

    template<typename T, typename R>
    vector<R> find(vector<T> &vec, T val) {
        auto r = vector<R>();
        for (int i = 0; i < vec.size(); i++) if (vec[i] == val) r.push_back(i);
        return r;
    }
};

#endif
