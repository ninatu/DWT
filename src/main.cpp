#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <ctime>
#include <iomanip>
#include <list>

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

    std::ifstream data_file(data_path.c_str());
    if (! data_file.is_open()) {
        utils::print("Can't open file!");
        throw std::exception();
    }

    std::vector<SpeechTs> tss = utils::readChromaTs(data_file);

    std::list<CentParam> cent_params;
    cent_params.push_back({41, 10});
    cent_params.push_back({121, 30});
    cent_params.push_back({271, 90});
    double dtw_val2 = dtw::msDtw(tss[0], tss[1], 30, cent_params).dtw;
    utils::print(dtw_val2);

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