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
#include <unistd.h>

#include "common.hpp"
#include "utils.hpp"
#include "pruned_dtw.hpp"
#include "fast_dtw.hpp"
#include "dtw.hpp"


template <typename T>
using Algorithm = std::function<DtwAnswer(const std::vector<T>&, const std::vector<T>&)>;

template <typename T>
void runAlgorithm(int argc, char **argv) {

}

void printUsageMessage() {
    std::cerr << "Usage: " << " algorith(dtw|pruned_dtw|fast_dtw) part_to_input_file" << std::endl;
}


int main(int argc, char *argv[]) {
    std::string data_type;
    std::string algorithm;
    std::string input_path;
    std::string output_path;
    try {
        if (getopt(argc, argv, "h:") != -1) {
            printUsageMessage();
        }


        if (argc >= 4) {
            algorithm = argv[1];
            data_type = argv[2];
            input_path = argv[3];
            output_path = argv[4];

            if (data_type != "double" and data_type != "vector") {
                throw std::invalid_argument("Invalid data_type param.");
            }

            if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw") {
                throw std::invalid_argument("Invalid algorithm param.");
            }

        } else {
            throw std::invalid_argument("Invalid number of parameters.");
        }
    } catch (std::exception &e) {
        printUsageMessage();
        utils::print(e.what());
        return -1;
    }

    std::vector<SpeechTs> tss = utils::readChromaTs(input_path);

    if (algorithm == "dtw") {
        utils::computePairWiseDtw<SpeechTsElem>(
                [](const SpeechTs &ts1, const SpeechTs &ts2){ return dtw::dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist);},
                tss,
                output_path);
    } else if (algorithm == "fast_dtw") {
        int radius = 30;
        std::list<CentParam> cens_params;
        cens_params.push_back({41, 10});
        cens_params.push_back({121, 30});
        cens_params.push_back({271, 90});

        utils::computePairWiseDtw<SpeechTsElem>(
                [radius, cens_params](const SpeechTs &ts1, const SpeechTs &ts2){ return dtw::msDtw(ts1, ts2, radius, cens_params);},
                tss,
                output_path);
    } else {
        std::cout << "Algorithm is not implemented yet... :(";
    }
    return 0;
}