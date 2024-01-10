#ifndef _ANALIZE_ALGORITHM_H_
#define _ANALIZE_ALGORITHM_H_

#include "objective_function.hpp"

#include <limits>
#include <atomic>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
#include <random>

using std::string;
using std::tuple;
using std::vector;

template <typename T, typename R>
class Algorithm {
public:
	Algorithm() {}
	Algorithm(const Algorithm<T, R>& o) {}
	~Algorithm() {}

	/**
	 * @brief info
	 * @return
	 */
	virtual string info() {
		return "";
	}
	/**
	 * @brief sinfo
	 * @return
	 */
	virtual string sinfo() {
		return "";
	}
	/**
	 * @brief run
	 * @param func
	 * @return
	 */
	virtual R run(ObjectiveFunction<T>& f) {
		return R();
	}

	std::default_random_engine rand_gen;

};

template<typename T>
class AnalAlgorithm : public Algorithm<T, tuple<vector<size_t>, vector<vector<size_t>>>> {
public:
	AnalAlgorithm() : Algorithm<T, tuple<vector<size_t>, vector<vector<size_t>>>>() {}
	AnalAlgorithm(const AnalAlgorithm<T>& o) : Algorithm<T, tuple<vector<size_t>, vector<vector<size_t>>>>(o) {}
	~AnalAlgorithm() {}

	/**
	 * @brief run
	 * @param func
	 * @return
	 */
	virtual tuple<vector<size_t>, vector<vector<size_t>>> run(BoundedObjectiveFunction<T>& f) {
		auto seps = vector<size_t>();
		auto allgroups = vector<vector<size_t>>();
		return std::make_tuple(seps, allgroups);
	}
};

#endif
