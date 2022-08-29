#ifndef _COMMON_FUNCS_H_
#define _COMMON_FUNCS_H_

#include <vector>
#include <algorithm>

/**
 * @brief vunion
 * @param a
 * @param b
 * @return
 */
template <typename T>
std::vector<T> vunion(std::vector<T> a, std::vector<T> b);

/**
 * @brief find
 * @param a
 * @param v
 * @return
 */
template<typename T>
int find(std::vector<T> a, T v);

/**
 * @brief find
 * @param a
 * @param v
 * @return
 */
template<typename T, typename R>
std::vector<R> find(std::vector<T>& a, T v);

/**
 * @brief sum
 * @param a
 * @return
 */
template <typename T>
T sum(std::vector<T> a);

/**
 * @brief have_intersection
 * @param a
 * @param b
 * @return
 */
template <typename T>
inline bool have_intersection(std::vector<T> a, std::vector<T> b);

/**
 * @brief copy_vector
 * @param src
 * @param dst
 * @param size
 * @return
 */
template <typename T>
T* copy_vector(T* src, T* dst, size_t size = 0);

/**
 * @brief copy_vector
 * @param src
 * @param size
 * @return
 */
template <typename T>
T* copy_vector(T* src, size_t size = 0);

/**
 * @brief copy_vector
 * @param src
 * @param dst
 * @param size
 * @return
 */
template <typename T>
std::vector<T> copy_vector(std::vector<T> src, std::vector<T> dst, size_t size = 0);

/**
 * @brief copy_vector
 * @param src
 * @param size
 * @return
 */
template <typename T>
std::vector<T> copy_vector(std::vector<T> src, size_t size = 0);

/**
 * @brief sortIndexWithQuickSort
 * @param array
 * @param first
 * @param last
 * @param index
 */
template <class VarType>
void sortIndexWithQuickSort(VarType array[], int first, int last, int index[]);

#endif
