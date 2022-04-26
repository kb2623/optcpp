#ifndef _TESTFUNC_H_
#define _TESTFUNC_H_

class TestFunc {
public:
    int dim;

    TestFunc(int idim) {
        dim = idim;
    };

    virtual void test_func(double*, double*, int) = 0;
};

class TestFuncBounds: public TestFunc {
public:
    double *x_bound_min;
    double *x_bound_max;
    int max_num_evaluations;

    TestFuncBounds(int dim, int g_max_num_evaluations): TestFunc(dim) {
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
