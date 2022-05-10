#ifndef _XDG_H_
#define _XDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"

#include <algorithm>

class XDG: public AnalizeAlgorithm {
public:
    XDG() {}
    ~XDG() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    double epsilon(double, double, double, double);

};

template <typename T>
T sum(vector<T> x) {
    T s = 0;
    for (auto e : x) s += e;
    return s;
};

template <typename T>
inline bool have_intersection(vector<T> a, vector<T> b) {
    for (auto e : a) for (auto ee : b) if (e == ee) return true;
    return false;
};

template <typename T>
inline vector<T> vunion(vector<T> a, vector<T> b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    auto c = vector<T>();
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(c));
    return c;
}

#endif
