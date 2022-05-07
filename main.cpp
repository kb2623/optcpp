#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include "cec17_test_func.hpp"
#include "sphere.hpp"
#include "search_algorithm.hpp"
#include "example.hpp"
#include "de.hpp"
#include "jde.hpp"
#include "jso.hpp"
#include "jsop.hpp"

void runAlgo(SearchAlgorithm *algo, TestFuncBounds &func) {
    std::cout << algo->sinfo() << " : ";
    auto r = algo->run(&func);
    for (auto e : std::get<1>(r)) std::cout << e << ' ';
    auto val = algo->eval(std::get<1>(r).data());
    std::cout << ": ";
    std::cout << std::get<0>(r) << " : ";
    if (val == std::get<0>(r)) std::cout << "OK";
    else std::cout << "ERROR >> " << val;
    std::cout << " : " << algo->get_nfes() - 1;
}

void runCEC(vector<SearchAlgorithm*> &algs) {
    size_t no_runs = 10;
    size_t g_problem_size = 10;
    for (int i = 0; i < 30; i++) try {
        size_t g_max_num_evaluations = g_problem_size * 10000;
        for (auto a : algs) {
            auto func = CEC17(g_problem_size, i, g_max_num_evaluations);
            for (int j = 0; j < no_runs; j++) {
                std::cout << "F" << i + 1 << " : ";
                runAlgo(a, func);
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
}

void runSphere(vector<SearchAlgorithm*> &algs) {
    size_t no_runs = 10;
    size_t g_problem_size = 5;
    size_t g_max_num_evaluations = g_problem_size * 10000;
    for (auto a : algs) {
        auto func = Sphere(g_problem_size, g_max_num_evaluations);
        for (int i = 0; i < no_runs; i++) {
            runAlgo(a, func);
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

int main() {
    const auto no_workers = 5;
    vector<SearchAlgorithm*> algs = {
        new DE(50, .9, .9, 1),
        new DE(50, .9, .9, no_workers),
        new jDE(50, 0.1, 0.1, 1),
        new jDE(50, 0.1, 0.1, no_workers),
        new jSO(50, 0.5, 0.5, 150),
        new jSOp(50, 0.5, 0.5, 150, no_workers),
        //new Bar(50, no_workers),
    };
    runCEC(algs);
    //runSphere(algs);
    return 0;
}
