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

template <typename T>
T* copy_vector(T* src, T* dst, size_t size) {
	if (dst == nullptr) return copy_vector<T>(src, size);
	for (size_t i = 0; i < size; i++) dst[i] = src[i];
	return dst;
}

template <typename T>
T* copy_vector(T* src, size_t size) {
	T* dst = new T[size];
	for (size_t i = 0; i < size; i++) dst[i] = src[i];
	return dst;
}

template <typename T>
std::vector<T> copy_vector(std::vector<T> src, std::vector<T> dst, size_t size) {
	if (size == 0) size = src.size();
	for (size_t i = 0; i < size; i++) if (dst.size() < i + 1) {
		dst.pop_back(src[i]);
	} else {
		dst[i] = src[i];
	}
	return dst;
}

template <typename T>
std::vector<T> copy_vector(std::vector<T> src, size_t size) {
	std::vector<T> dst(size);
	for (size_t i = 0; i < size; i++) dst[i] = src[i];
	return dst;
}

template<class VarType>
void sortIndexWithQuickSort(VarType array[], int first, int last, int index[]) {
	VarType x = array[(first + last) / 2];
	int i = first;
	int j = last;
	VarType temp_var = 0;
	int temp_num = 0;
	while (true) {
		while (array[i] < x) i++;
		while (x < array[j]) j--;
		if (i >= j) break;
		temp_var = array[i];
		array[i] = array[j];
		array[j] = temp_var;
		temp_num = index[i];
		index[i] = index[j];
		index[j] = temp_num;
		i++;
		j--;
	}
	if (first < (i - 1)) sortIndexWithQuickSort(array, first, i - 1, index);
	if ((j + 1) < last) sortIndexWithQuickSort(array, j + 1, last, index);
}

#endif
