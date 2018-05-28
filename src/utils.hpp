#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

#include "common.hpp"


template<typename T>
double compute_cpu_time(
        std::function<DtwAnswer(const std::vector<T> &, const std::vector<T> &)> algorithm,
        const std::vector<std::vector<T>> &tss
){
    std::clock_t c_start = std::clock();
    for (int i = 0; i < tss.size(); i++) {
        for (int j = i + 1; j < tss.size(); j++) {
            algorithm(tss[i], tss[j]);
        }
    }
    std::clock_t c_end = std::clock();
    return double(c_end - c_start) / (CLOCKS_PER_SEC);
}

namespace utils {
    template<typename Head>
    void print(Head &&h) {
        std::cout << std::forward<Head>(h) << std::endl;
    }

    template<typename Head, typename... Args>
    void print(Head &&h, Args &&... args) {
        std::cout << std::forward<Head>(h) << " ";
        print(std::forward<Args>(args)...);
    }
} // namespace utils


void printComandLineErrorMessage(int argc, char **argv);

std::vector<std::vector<double>> readInputFile(const std::string &path);

int argmin(double x0, double x1, double x2);



