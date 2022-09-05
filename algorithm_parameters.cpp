#include "algorithm_parameters.hpp"

// -------------------- AlgParams --------------------

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
T getParam(AlgParams& params, string key, T dval) {
	if (params.has(key)) {
		return params.at<T>(key);
	} else {
		return dval;
	}
}

template <typename T>
T getParam(AlgParams* params, string key, T dval) {
	if (params != nullptr && params->has(key)) {
		return params->at<T>(key);
	} else {
		return dval;
	}
}

// -------------------- RunAlgParams --------------------

template <typename T>
RunAlgParams<T>::RunAlgParams() : _no_gen(0) {
	f_best = std::numeric_limits<double>::max();
	x_best = std::vector<double>();
}

template <typename T>
RunAlgParams<T>::RunAlgParams(BoundedObjectiveFunction<T>& fitf) : _no_gen(0), _fitf(fitf) {
	f_best = std::numeric_limits<double>::max();
	x_best = std::vector<double>();
}

template <typename T>
RunAlgParams<T>::~RunAlgParams() {}

template <typename T>
ulongint RunAlgParams<T>::operator++() {
	_no_gen++;
	return _no_gen;
}

template <typename T>
void RunAlgParams<T>::setBestSolution(T* x, double f) {
	if (f_best > f) {
		f_best = f;
		for (size_t i = 0; i < _fitf.dim(); i++) x_best[i] = x[i];
	}
}

template <typename T>
void RunAlgParams<T>::setBestSolution(vector<T> x, double f) {
	setBestSolution(&x[0], f);
}

template <typename T>
tuple<double, vector<T>> RunAlgParams<T>::getBestSolution() {
	return std::make_tuple(f_best, x_best);
}

template <typename T>
T* RunAlgParams<T>::makeNewArrayIndividual(thread_data& tdata) {
	double* x = new T[_fitf.dim()];
	for (int i = 0; i < _fitf.dim(); i++) x[i] = _fitf.gen(i, tdata);
	return x;
}

template <typename T>
vector<T> RunAlgParams<T>::makeNewVectorIndividual(thread_data& tdata) {
	vector<T> x;
	for (int i = 0; i < _fitf.dim(); i++) x.push_back(_fitf.gen(i, tdata));
	return x;
}

template <typename T>
BoundedObjectiveFunction<T>& RunAlgParams<T>::fitf() const {
	return _fitf;
}

template <typename T>
bool RunAlgParams<T>::max_no_fes() {
	return false;
}

template <typename T>
bool RunAlgParams<T>::max_no_gen() {
	return false;
}

template <typename T>
bool RunAlgParams<T>::target_value() {
	return false;
}

// -------------------- RunParallelAlgParams --------------------

template <typename T>
RunParallelAlgParams<T>::RunParallelAlgParams() {}

template <typename T>
RunParallelAlgParams<T>::RunParallelAlgParams(Barrier& b) : sync(b) {}

template <typename T>
RunParallelAlgParams<T>::~RunParallelAlgParams() {}

template <typename T>
void RunParallelAlgParams<T>::arrive_and_wait() {
	sync.arrive_and_wait();
}

template <typename T>
void RunParallelAlgParams<T>::setBestSolution(T* x, double f) {
	best_lock.lock();
	RunAlgParams<T>::setBestSolution(x, f);
	best_lock.unlock();
}
