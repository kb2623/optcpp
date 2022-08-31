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

	/**
	 * @brief getParamsVals
	 * @return
	 */
	map<string, any>& getParamsVals();
	/**
	 * @brief setParamsVals
	 * @param params
	 */
	void setParamsVals(map<string, any> params);
	/**
	 * @brief setParamVal
	 * @param key
	 * @param value
	 */
	void setParamVal(string key, any value);
	/**
	 * @brief setParamVal
	 * @param key
	 * @param pvalue
	 */
	void setParamVal(string key, any* pvalue);
	/**
	 * @brief has
	 * @param key
	 * @return
	 */
	bool has(string key);
	/**
	 * @brief operator []
	 * @param key
	 * @return
	 */
	any& operator[](string key);
	/**
	 * @brief operator ()
	 * @param key
	 * @return
	 */
	any operator()(string key);
	/**
	 * @brief operator []
	 * @param key
	 * @return
	 */
	template <typename T> T& operator[](string key);
	/**
	 * @brief operator ()
	 * @param key
	 * @return
	 */
	template <typename T> T operator()(string key);
	/**
	 * @brief at
	 * @param key
	 * @return
	 */
	template <typename T> T at(string key);

protected:
	/**
	 * @brief params
	 */
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
