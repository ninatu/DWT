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
#include "sparse_dtw.hpp"
#include "dtw.hpp"


int main(int argc, char *argv[]) {
    std::string data_type;
    std::string algorithm;
    std::string input_path;
    std::string output_path;
    int window = -1;
    double res = 0.5;
    int radius = 30;
    int scale = 2;

    try {
        if (argc >= 5) {
            data_type = argv[1];
            input_path = argv[2];
            output_path = argv[3];
            algorithm = argv[4];

            if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw"
                    and algorithm != "sparse_dtw" and algorithm != "ms_dtw") {
                throw std::invalid_argument("Invalid algorithm param.");
            }

            int option_argv_offset = 5;
            int option_argc = argc - option_argv_offset;

            if(algorithm == "pruned_dtw" and option_argc > 0) {
                std::stringstream str_line(argv[option_argv_offset]);
                str_line >> window;
            }
            else if(algorithm == "sparse_dtw" and option_argc > 0){
                std::stringstream str_line(argv[option_argv_offset]);
                str_line >> res;
            }
            else if(algorithm == "fast_dtw" and option_argc > 0){
                if (option_argc == 2) {
                    std::stringstream str_line(argv[option_argv_offset]);
                    str_line >> radius;
                    str_line = std::stringstream (argv[option_argv_offset + 1]);
                    str_line >> scale;
                } else{
                    throw std::invalid_argument("Invalid fast_dtw algorithm parameters.");
                }
            }
            if (data_type != "double" and data_type != "vector") {
                throw std::invalid_argument("Invalid data_type param.");
            }
        } else {
            throw std::invalid_argument("Invalid number of parameters.");
        }
    } catch (std::exception &e) {
        utils::print(e.what());
        return -1;
    }

    std::vector<SpeechTs> tss_v;
    std::vector<DoubleTs> tss_d;

    if (data_type == "vector") {
        tss_v = utils::readChromaTs(input_path);
    } else {
        tss_d = utils::read_UCR_TS_Archive_2015(input_path);
    }


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
        if (data_type == "vector") {
            throw std::invalid_argument("Not emplemented fast_dtw for vector type data");
        } else {
            utils::computePairWiseDtw<double>(
                    [radius, scale](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::fastDtw<double>(ts1, ts2, radius, scale, dtw::reduceDoubleTs, getDoubleTsElemDist);
                    },
                    tss_d,
                    output_path);
        }
    } else if (algorithm == "ms_dtw") {
            std::list<CentParam> cens_params;
            cens_params.push_back({41, 10});
            cens_params.push_back({121, 30});
            cens_params.push_back({271, 90});

            if (data_type == "vector") {
                utils::computePairWiseDtw<SpeechTsElem>(
                        [radius, cens_params](const SpeechTs &ts1, const SpeechTs &ts2){ return dtw::msDtw(ts1, ts2, radius, cens_params);},
                        tss_v,
                        output_path);
            }
            else{
                throw std::invalid_argument("ms_dtw is used only for vector data");
            }
    } else if (algorithm == "sparse_dtw") {
        if (data_type == "vector") {
            utils::computePairWiseDtw<SpeechTsElem>(
                    [res](const SpeechTs &ts1, const SpeechTs &ts2) {
                        return dtw::sparse_dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist, res);
                    },
                    tss_v,
                    output_path);
        }
        else{

            utils::computePairWiseDtw<double>(
                    [res](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::sparse_dtw<double>(ts1, ts2, getDoubleTsElemDist, res);
                    },
                    tss_d,
                    output_path);
        }
        
    } else if (algorithm == "pruned_dtw") {
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
