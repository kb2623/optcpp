#include "algorithm_parameters.hpp"

AlgParams::AlgParams() {}

AlgParams::~AlgParams() {}

void AlgParams::setParamsVals(map<string, any> params) {
	for (auto [k, v] : params) this->params[k] = v;
}

void AlgParams::setParamVal(string k, any v) {
	params[k] = v;
}

void AlgParams::setParamVal(string k, any* v) {
	params[k] = v;
}

map<string, any>& AlgParams::getParamsVals() {
	return params;
}

bool AlgParams::has(string key) {
	return params.find(key) != params.end();
}

any& AlgParams::operator[](string key) {
	if (params.find(key) != params.end()) return params[key];
	else throw std::invalid_argument("Bad key '" + key + "'.");
}

any AlgParams::operator()(string key) {
	if (params.find(key) != params.end()) return params[key];
	else return any();
}

template <typename T>
T& AlgParams::operator[](string key) {
	return std::any_cast<T>(operator[](key));
}

template <typename T>
T AlgParams::operator()(string key) {
	return std::any_cast<T>(operator()(key));
}

template <typename T>
T AlgParams::at(string key) {
	return std::any_cast<T>(operator()(key));
}

template <typename T>
T getParam(AlgParams* params, string key, T dval) {
	if (params != nullptr && params->has(key)) {
		return params->at<T>(key);
	} else {
		return dval;
	}
}
