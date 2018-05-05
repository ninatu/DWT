#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <ctime>
#include <iomanip>

#include "utils.hpp"
#include "pruned_dtw.hpp"
#include "dtw.hpp"


using Algorithm = std::function<double(const std::vector<double> &, const std::vector<double> &, int)>;

double compute_cpu_time(Algorithm algorithm, const std::vector<std::vector<double>> &tss, double warping_window_width) {
    std::clock_t c_start = std::clock();
    for (int i = 0; i < tss.size(); i++) {
        for (int j = i + 1; j < tss.size(); j++) {
            int wc = int(warping_window_width * tss[j].size());
            algorithm(tss[i], tss[j], wc);
        }
    }
    std::clock_t c_end = std::clock();
    return double(c_end - c_start) / (CLOCKS_PER_SEC);
}


int main(int argc, char *argv[]) {
    std::string algorithm;
    std::string data_path;
    if (argc == 3) {
        algorithm = argv[1];
        data_path = argv[2];
        if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw") {
            printComandLineErrorMessage(argc, argv);
            return -1;
        }
    } else {
        printComandLineErrorMessage(argc, argv);
        return -1;
    }

    std::vector<std::vector<double>> tss = readInputFile(data_path);

    if (algorithm == "dtw") {
        double sec = compute_cpu_time(dtw, tss, 0);
        std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
    } else if (algorithm == "pruned_dtw") {
        double sec = compute_cpu_time(pruned_dtw, tss, 0);
        std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
    } else {
        std::cout << "Algorithm is not implemented yet... :(";
    }
    return 0;
}