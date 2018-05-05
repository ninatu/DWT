#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

constexpr double INF = 1e20;

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

double cost(double x, double y);