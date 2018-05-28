#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <ctime>
#include <iomanip>

#include "common.hpp"
#include "utils.hpp"
#include "pruned_dtw.hpp"
#include "fast_dtw.hpp"
#include "dtw.hpp"


using Algorithm = std::function<DtwAnswer(const std::vector<double> &, const std::vector<double> &)>;

double compute_cpu_time(Algorithm algorithm, const std::vector<std::vector<double>> &tss) {
    std::clock_t c_start = std::clock();
    for (int i = 0; i < tss.size(); i++) {
        for (int j = i + 1; j < tss.size(); j++) {
            algorithm(tss[i], tss[j]);
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

//    for(int i = 0; i < tss.size(); i++) {
//        for(int j = i + 1; j < tss.size(); j++) {
//            DoubleTs ts1 = tss[i];
////    ts1.resize(20);
//            DoubleTs ts2 = tss[j];
////    ts2.resize(20);
//
////    utils::print("ts1", ts1[0], ts1[1]);
////    utils::print("ts2", ts2[0], ts2[1]);
//
//            double dtw_val = dtw::prunedDtw<double>(ts1, ts2, getDist, 10).dtw;
////            utils::print("=============");
//            double dtw_val2 = dtw::dtw<double>(ts1, ts2, getDist, 10).dtw;
////            double dtw_val2 = dtw::fastDtw<double>(ts1, ts2, 2, 2, dtw::reduce_double_ts, getDist).dtw;
//            utils::print(i, j);
//            if (std::abs(dtw_val - dtw_val2) > 1e-6) {
//                utils::print(i, j, "vals:", dtw_val, dtw_val2);
//                throw std::exception();
//            }
//
//        }
//    }

//    if (algorithm == "dtw") {
//        double sec = compute_cpu_time(dtw, tss, 0);
//        std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
//    } else if (algorithm == "pruned_dtw") {
//        double sec = compute_cpu_time(pruned_dtw, tss, 0);
//        std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
//    } else {
//        std::cout << "Algorithm is not implemented yet... :(";
//    }
    return 0;
}