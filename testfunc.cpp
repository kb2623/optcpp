#include "testfunc.hpp"

TestFunc::TestFunc(size_t dim) {
    this->dim = dim;
}

TestFuncBounds::TestFuncBounds(size_t dim, size_t g_max_num_evaluations) : TestFunc(dim) {
    x_bound_min = new double[dim];
    x_bound_max = new double[dim];
    max_num_evaluations = g_max_num_evaluations;
}

TestFuncBounds::~TestFuncBounds() {
    if (x_bound_max != nullptr) {
        delete [] x_bound_max;
        x_bound_max = nullptr;
    }
    if (x_bound_min != nullptr) {
        delete [] x_bound_min;
        x_bound_min = nullptr;
    }
}
