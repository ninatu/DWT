#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "utils.hpp"
#include "pruned_dtw.hpp"
#include "dtw.hpp"

//std::vector<std::vector<double>> dtw(const std::vector<std::vector<double>> &tss) {
//    std::vector<std::vector<double>> dist;
//
//}

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
//        int i = 0, j = 2;
//        std::cout << tss.at(i).size() << " " <<  tss.at(j).size() << std::endl;
//        std::cout << "DTW:" << dtw(tss.at(i), tss.at(j), 300) << std::endl;
//        std::cout << "Pruned DTW:" << pruned_dtw(tss.at(i), tss.at(j), 300) << std::endl;
        for (int i = 0; i < tss.size(); i++) {
            std::cout << i << std::endl;
            for (int j = i + 1; j < tss.size(); j++) {
                double x = dtw(tss.at(i), tss.at(j), 30);
                double y = pruned_dtw(tss.at(i), tss.at(j), 30);
                if (std::abs(x - y) > 1e-6) {
                    std::cout << i << " " << j << std::endl;
                    std::cout << "DTW:" << x << std::endl;
                    std::cout << "Pruned DTW:" << y << std::endl;
//                    throw std::exception();
                }
            }
        }

    } else if (algorithm == "pruned_dtw") {
        std::cout << "Pruned DTW:" << pruned_dtw(tss.at(0), tss.at(1)) << std::endl;
    } else {
        std::cout << "Algorithm is not implemented yet... :(";
    }
    return 0;
}