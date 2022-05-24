#ifndef _COMMON_FUNCS_H_
#define _COMMON_FUNCS_H_

#include <vector>
#include <algorithm>

template <typename T>
std::vector<T> vunion(std::vector<T> s1, std::vector<T> s2) {
    std::vector<T> tmp;
    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());
    std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(tmp));
    return tmp;
}

template<typename T>
int find(std::vector<T> s, T e) {
    for (size_t i = 0; i < s.size(); i++) if (s[i] == e) return i;
    return -1;
}

template<typename T, typename R>
std::vector<R> find(std::vector<T> &vec, T val) {
    auto r = std::vector<R>();
    for (int i = 0; i < vec.size(); i++) if (vec[i] == val) r.push_back(i);
    return r;
}

template <typename T>
T sum(std::vector<T> x) {
    T s = 0;
    for (auto e : x) s += e;
    return s;
}

template <typename T>
inline bool have_intersection(std::vector<T> a, std::vector<T> b) {
    for (auto e : a) for (auto ee : b) if (e == ee) return true;
    return false;
}

#endif
