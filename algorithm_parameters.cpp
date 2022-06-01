#include "algorithm_parameters.hpp"

AlgParams::AlgParams() {}

AlgParams::~AlgParams() {}

void AlgParams::setParamsVals(map<string, any> params) {
	for (auto [k, v] : params) this->params[k] = v;
}

void AlgParams::setParamVal(string k, any v) {
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
