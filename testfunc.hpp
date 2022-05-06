#ifndef _TESTFUNC_H_
#define _TESTFUNC_H_

#include <cstddef>

class TestFunc {
public:
    size_t dim;

    TestFunc(size_t idim) {
        dim = idim;
    };

    virtual void test_func(double*, double*, int) = 0;
};

class TestFuncBounds: public TestFunc {
public:
    double *x_bound_min;
    double *x_bound_max;
    size_t max_num_evaluations;

    TestFuncBounds(size_t dim, size_t g_max_num_evaluations): TestFunc(dim) {
        x_bound_min = new double[dim];
        x_bound_max = new double[dim];
        max_num_evaluations = g_max_num_evaluations;
    }
    ~TestFuncBounds() {
        delete[] x_bound_min;
        delete[] x_bound_max;
    }
};

#endif
