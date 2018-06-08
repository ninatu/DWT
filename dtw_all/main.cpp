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
#include "sparce_dtw.hpp"
//#include "sparce_dtw.cpp"
#include "dtw.hpp"


template <typename T>
using Algorithm = std::function<DtwAnswer(const std::vector<T>&, const std::vector<T>&)>;

template <typename T>
void runAlgorithm(int argc, char **argv) {

}

void printUsageMessage() {
    std::cerr << "Usage: " << " algorith(dtw|pruned_dtw|fast_dtw|sparce_dtw|new) part_to_input_file" << std::endl;
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
            std::cout << algorithm;
            data_type = argv[2];
            input_path = argv[3];
            output_path = argv[4];

            if (data_type != "double" and data_type != "vector") {
                throw std::invalid_argument("Invalid data_type param.");
            }

            if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw" and algorithm != "sparce_dtw") {
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

    std::vector<SpeechTs> tss_v;
    std::vector<DoubleTs> tss_d;

   // std::vector<std::string> docs = {"Brahms_Op034b-03.txt", "Brahms_Op108-01.txt","Brahms_Op108-02.txt","Brahms_Op108-03.txt","Brahms_Op108-04.txt","Chopin_Op060.txt","Debussy_L140-01.txt","Debussy_L140-02.txt","Debussy_L140-03.txt","Martin_Trio-02.txt","Mozart_KV448-01.txt","Rachmaninoff_Op005-01.txt","Rachmaninoff_Op005-04.txt","Ravel_PianoTrio-01.txt","Ravel_PianoTrio-02.txt","Ravel_PianoTrio-03.txt","Ravel_PianoTrio-04.txt"};

    if (data_type == "vector") {
    tss_v = utils::readChromaTs(input_path);
    }
    else{
        tss_d = utils::read_UCR_TS_Archive_2015(input_path);
    }
   // std::cout << tss.size() << '\n';

    if (algorithm == "dtw") {
        if (data_type == "vector") {
            utils::computePairWiseDtw<SpeechTsElem>(
                    [](const SpeechTs &ts1, const SpeechTs &ts2) {
                        return dtw::dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist);
                    },
                    tss_v,
                    output_path);
        }
        else{
            utils::computePairWiseDtw<double>(
                    [](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::dtw<double>(ts1, ts2, getDoubleTsElemDist);
                    },
                    tss_d,
                    output_path);
        }
    } else if (algorithm == "fast_dtw") {
        int radius = 30;
        int scale = 2;

        std::list<CentParam> cens_params;
        cens_params.push_back({41, 10});
        cens_params.push_back({121, 30});
        //10104.97, time: 7.44
        //0, 1: dtw: 10935.16, time: 4.95
        cens_params.push_back({271, 90});

        //sparce 0, 1: dtw: 11747.47, time: 4.63
        //0, 1: dtw: 10843.77, time: 6.70
       // 0, 1: dtw: 11747.47, time: 5.24
        //0, 1: dtw: 11747.47, time: 4.55
        if (data_type == "vector") {
        utils::computePairWiseDtw<SpeechTsElem>(
                [radius, cens_params](const SpeechTs &ts1, const SpeechTs &ts2){ return dtw::msDtw(ts1, ts2, radius, cens_params);},
                tss_v,
                output_path);
        }
        else{
            utils::computePairWiseDtw<double>(
                    [radius, cens_params, scale](const DoubleTs &ts1, const DoubleTs &ts2){ return dtw::fastDtw<double>(ts1, ts2, radius, scale, dtw::reduceDoubleTs, getDoubleTsElemDist);},
                    tss_d,
                    output_path);
            //std::cout << "Algorithm is not implemented yet... :(";
        }

    } else if (algorithm == "sparce_dtw") {

        double Res = 0.5;
        //DtwAnswer dtw_a = dtw::sparce_dtw<SpeechTsElem>(tss[0], tss[1], getSpeechTsElemDist, Res);
        //std::cout << dtw_a.dtw;
        if (data_type == "vector") {
            utils::computePairWiseDtw<SpeechTsElem>(
                    [Res](const SpeechTs &ts1, const SpeechTs &ts2) {
                        return dtw::sparce_dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist, Res);
                    },
                    tss_v,
                    output_path);
        }
        else{
            utils::computePairWiseDtw<double>(
                    [Res](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::sparce_dtw<double>(ts1, ts2, getDoubleTsElemDist, Res);
                    },
                    tss_d,
                    output_path);
        }
        
    } else if (algorithm == "pruned_dtw") {

        int window = -1;
        //DtwAnswer dtw_a = dtw::sparce_dtw<SpeechTsElem>(tss[0], tss[1], getSpeechTsElemDist, Res);
        //std::cout << dtw_a.dtw;
        if (data_type == "vector") {
            utils::computePairWiseDtw<SpeechTsElem>(
                    [window](const SpeechTs &ts1, const SpeechTs &ts2) {
                        return dtw::prunedDtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist, window);
                    },
                    tss_v,
                    output_path);
        }
        else{
            utils::computePairWiseDtw<double>(
                    [window](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::prunedDtw<double>(ts1, ts2, getDoubleTsElemDist, window);
                    },
                    tss_d,
                    output_path);
        }

    }
    else {
        std::cout << "Algorithm is not implemented yet... :(";
    }
    return 0;
}
