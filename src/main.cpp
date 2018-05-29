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


int main(int argc, char *argv[]) {
    std::string algorithm;
    std::string data_path;
    if (argc == 3) {
        algorithm = argv[1];
        data_path = argv[2];
        if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw") {
            utils::printComandLineErrorMessage(argc, argv);
            return -1;
        }
    } else {
        utils::printComandLineErrorMessage(argc, argv);
        return -1;
    }

    std::vector<std::vector<double>> tss = utils::readInputFile(data_path);

    for(int i = 0; i < tss.size(); i++) {
        for(int j = i + 1; j < tss.size(); j++) {
            DoubleTs ts1 = tss[i];
//    ts1.resize(20);
            DoubleTs ts2 = tss[j];
//    ts2.resize(20);

//    utils::print("ts1", ts1[0], ts1[1]);
//    utils::print("ts2", ts2[0], ts2[1]);


//            double dtw_val = dtw::prunedDtw<double>(ts1, ts2, getDist, 10).dtw;
            Path path = dtw::prunedDtw<double>(ts1, ts2, getDoubleTsElemDist, 10).path;
//            utils::print("=============");
//            double dtw_val2 = dtw::fastDt<double>(ts1, ts2, getDist, 10).dtw;
            for (auto elem : path) {
                utils::print(elem.i ,elem.j);
            }
            utils::print("=====================");

//            double dtw_val2 = dtw::fastDtw<double>(ts1, ts2, 2, 2, dtw::reduceDoubleTs, getDist).dtw;
            path = dtw::fastDtw<double>(ts1, ts2, 2, 2, dtw::reduceDoubleTs, getDoubleTsElemDist).path;
            for (auto elem : path) {
                utils::print(elem.i ,elem.j);
            }
            break;

//            if (std::abs(dtw_val - dtw_val2) > 1e-6) {
//                utils::print(i, j, "vals:", dtw_val, dtw_val2);
//                throw std::exception();
//            }

        }
        break;
    }

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