#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include "cec17_test_func.hpp"
#include "search_algorithm.hpp"
#include "example.hpp"
#include "jso.hpp"
#include "jsop.hpp"
#include "de.hpp"

void runAlgo(SearchAlgorithm *algo, TestFuncBounds &func) {
    std::cout << algo->info() << " : ";
    auto r = algo->run(&func);
    std::cout << std::get<0>(r) << " : ";
    for (auto e : std::get<1>(r)) std::cout << e << ' ';
    auto val = algo->eval(std::get<1>(r).data());
    std::cout << ": ";
    if (val == std::get<0>(r)) std::cout << "OK";
    else std::cout << "ERROR >> " << val;
}

int main() {
    const auto no_workers = 6;
    SearchAlgorithm *algs[] = {
        new DE(50, .9, .5, no_workers),
        new jSOp(50, 0.5, 0.5, 150, no_workers),
        new Bar(50, no_workers),
        new DE(50, .9, .5, 1),
        new jSO(50, 0.5, 0.5, 150)
    };
    for (int i = 0; i < 30; i++) try {
        int g_problem_size = 30;
        int g_max_num_evaluations = g_problem_size * 10000;
        for (int j = 0; j < std::ssize(algs); j++) {
            auto func = CEC17(g_problem_size, i, g_max_num_evaluations);
            std::cout << "F" << i + 1 << " : ";
            runAlgo(algs[j], func);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
    return 0;
}
