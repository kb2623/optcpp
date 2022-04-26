#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include "search_algorithm.hpp"
#include "cec17_test_func.hpp"
#include "jso.hpp"
#include "jsop.hpp"

class Bar : public SearchAlgorithm {
public:
    std::barrier<> *sync_point;
    size_t no_thr;

    size_t np;
    std::vector<std::vector<double>> x;
    std::vector<double> xf;

    Bar(size_t ino_thr, size_t inp = 50) {
        nfes = 0;
        this->no_thr = ino_thr;
        this->np = inp;
    };

    virtual std::string info() override {
        return "Testtni algo";
    };

    virtual std::tuple<double, std::vector<double>> run(TestFuncBounds* ifun) override {
        initRun(ifun);
        sync_point = new std::barrier(no_thr);
        std::vector<std::thread> threads;
        for (int i = 1; i < no_thr; i++) {
            threads.emplace_back(std::thread(&Bar::start, this, i));
        }
        start(0);
        for (auto& thread : threads) {
            thread.join();
        }
        return std::make_tuple(0.0, std::vector<double>(ifun->dim, 0));
    };

    void initRun(TestFuncBounds *ifunc) {
        SearchAlgorithm::initRun(ifunc);
        x = std::vector<std::vector<double>>(np, std::vector<double>(func->dim));
        xf = std::vector<double>(np);
        for (int i = 0; i < np; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
        for (int i = 0; i < np; i++) xf[i] = eval(x[i].data());
    }

    void start(int id) {
        int s = np / no_thr;
        while (nfes < func->max_num_evaluations) {
            for (int i = s * id; i < np && i < (id + 1) * s; i++) for (int j = 0; j < func->dim; j++) x[i][j] = func->x_bound_max[j] - func->x_bound_min[j] * randDouble() + func->x_bound_min[j];
            for (int i = s * id; i < np && i < (id + 1) * s; i++) xf[i] = eval(x[i].data());
            sync_point->arrive_and_wait();
        }
        sync_point->arrive_and_wait();
    };
};

int main() {
    const auto no_workers = 10;
    for (int i = 0; i < 30; i++) try {
        std::cout << "Function " << i + 1 << std::endl;
        int g_problem_size = 10;
        int g_max_num_evaluations = g_problem_size * 10000;
        int g_pop_size = int(round(sqrt(g_problem_size)) * log(g_problem_size) * 25);
        int g_memory_size = 5;
        double g_arc_rate = 1;
        double g_p_best_rate = 0.25;
        auto func = CEC17(g_problem_size, i, g_max_num_evaluations);
        //auto jso = jSO(g_pop_size, g_arc_rate, g_p_best_rate, g_memory_size);
        //auto r1 = jso.run(&func);
        //std::cout << std::get<0>(r1) << " < - > ";
        //for (auto e : std::get<1>(r1)) std::cout << e << ' ';
        //std::cout << std::endl;
        auto jsop = jSOp(g_pop_size, g_arc_rate, g_p_best_rate, g_memory_size, 10);
        auto r2 = jsop.run(&func);
        std::cout << std::get<0>(r2) << " < - > ";
        for (auto e : std::get<1>(r2)) std::cout << e << ' ';
        std::cout << std::endl;
        //auto work = Bar(no_workers, 50);
        //auto r3 = work.run(&func);
        //std::cout << std::get<0>(r3) << " < - > ";
        //for (auto e : std::get<1>(r3)) std::cout << e << ' ';
        std::cout << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
    return 0;
}
