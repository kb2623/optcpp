#ifndef _TESTFUNC_H_
#define _TESTFUNC_H_

#include <cstddef>

class TestFunc {
public:
    TestFunc(size_t);

    virtual void test_func(double*, double*, int) = 0;

    size_t dim;
};

class TestFuncBounds: public TestFunc {
public:
    TestFuncBounds(size_t, size_t);
    ~TestFuncBounds();

    double *x_bound_min;
    double *x_bound_max;
    size_t max_num_evaluations;
};

#endif
