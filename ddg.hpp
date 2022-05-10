#ifndef _DDG_H_
#define _DDG_H_

#include "analize_algorithm.hpp"
#include "testfunc.hpp"

#include <algorithm>

class DDG: public AnalizeAlgorithm {
public:
    double epsilon_multi = 10e-8;

    DDG() {}
    ~DDG() {}

    virtual tuple<vector<unsigned int>, vector<vector<unsigned int>>> run(TestFuncBounds*) override;
    virtual string info() override;
    virtual string sinfo() override;

protected:
    double get_epsilon_addi(double, double, double, double);

};

template<typename T>
vector<T> vuinon(vector<T> s1, vector<T> s2) {
    vector<T> tmp;
    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());
    std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(tmp));
	return tmp;
}

template<typename T>
int find(vector<T> s, T e) {
    for (unsigned int i = 0; i < s.size(); i++) if (s[i] == e) return i;
    return -1;
}

#endif
