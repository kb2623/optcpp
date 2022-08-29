#ifndef _ALGORITHM_PARAMETERS_H_
#define _ALGORITHM_PARAMETERS_H_

#include <map>
#include <any>
#include <vector>
#include <string>
#include <stdexcept>

using std::any;
using std::map;
using std::string;
using std::vector;

class AlgParams {
public:
	AlgParams();
	~AlgParams();

	map<string, any>& getParamsVals();
	void setParamsVals(map<string, any>);
	void setParamVal(string, any*);
	void setParamVal(string, any);
	bool has(string key);
	any& operator[](string key);
	template<typename T> T& operator[](string key) {
		if (params.find(key) != params.end()) return std::any_cast<T>(params[key]);
		else throw std::invalid_argument("Bad key '" + key + "'.");
	}
	any operator()(string key);
	template<typename T> T operator()(string key) {
		if (params.find(key) != params.end()) return std::any_cast<T>(params[key]);
		else throw std::invalid_argument("Bad key '" + key + "'.");
	}
	template<typename T> T at(string key) {
		if (params.find(key) != params.end()) return std::any_cast<T>(params[key]);
		else throw std::invalid_argument("Bad key '" + key + "'.");
	}

protected:
	map<string, any> params;
};

/**
 * @brief getParam
 * @param params
 * @param key
 * @param dval
 * @return
 */
template <typename T>
T getParam(AlgParams* params, string key, T dval);

#endif
