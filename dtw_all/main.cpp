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
    std::cerr << "Usage: " << " algorith(dtw|pruned_dtw <window>|fast_dtw <radius scale>|sparse_dtw <Res>) type(double|vector) part_to_input_file path_to_output_file\n" << std::endl;
}

bool legal_int(char *str)
{
    while (*str != '\0')
    {
        if (!isdigit(*str) and !(*str=='e') and !(*str=='.'))
        {
           // std::cout <<"not" << *str;
            return false;
        }
        str++;
    }
    return true;
}



int main(int argc, char *argv[]) {
    std::string data_type;
    std::string algorithm;
    std::string input_path;
    std::string output_path;
    int window = -1;
    double Res = 0.5;
    int radius = 30;
    int scale = 2;

    try {
        if (getopt(argc, argv, "h:") != -1) {
            printUsageMessage();
        }


        if (argc >= 4) {
            algorithm = argv[1];
            //std::cout << algorithm;
            //data_type = argv[2];
            //input_path = argv[3];
            //output_path = argv[4];


            if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw" and algorithm != "sparse_dtw") {
                throw std::invalid_argument("Invalid algorithm param.");
            }

            int start_in = 2;

            if (algorithm == "dtw"){
                start_in = 2;
            }
            else if(algorithm == "pruned_dtw"){

                if (legal_int(argv[2])){


                    std::stringstream str_line(argv[2]);

                    str_line >> window;
                    start_in = 3;
                }

               // std::cout << window<< "\n";


            }
            else if(algorithm == "sparse_dtw"){
                if (legal_int(argv[2])){


                    std::stringstream str_line(argv[2]);

                    str_line >> Res;
                    start_in = 3;
                }

              //  std::cout << Res<< "\n";

            }
            else if(algorithm == "fast_dtw"){

                bool is_int = false;
                if (legal_int(argv[2])){
                    std::stringstream str_line(argv[2]);
                    str_line >> radius;
                    is_int = true;

                }

                if (legal_int(argv[3])){
                    std::stringstream str_line(argv[3]);

                    str_line >> scale;
                    start_in = 4;
                }
                else{

                    if (is_int){
                        throw std::invalid_argument("Invalid fast_dtw algorithm parameters.");
                    }
                }
                //std::cout << radius << "\n";
               // std::cout << scale<< "\n";
            }
            if (argc < start_in+3) {
                throw std::invalid_argument("Invalid input. Type, part_to_input_file, path_to_output_file parameters are missed.");
            }
            data_type = argv[start_in];
            input_path = argv[start_in+1];
            output_path = argv[start_in+2];

            if (data_type != "double" and data_type != "vector") {
                throw std::invalid_argument("Invalid data_type param.");
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
            utils::computePairWiseDtw<double>(
                    [radius, cens_params, scale](const DoubleTs &ts1, const DoubleTs &ts2){ return dtw::fastDtw<double>(ts1, ts2, radius, scale, dtw::reduceDoubleTs, getDoubleTsElemDist);},
                    tss_d,
                    output_path);
        }

    } else if (algorithm == "sparse_dtw") {

       // double Res = 0.5;

        if (data_type == "vector") {
            utils::computePairWiseDtw<SpeechTsElem>(
                    [Res](const SpeechTs &ts1, const SpeechTs &ts2) {
                        return dtw::sparse_dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist, Res);
                    },
                    tss_v,
                    output_path);
        }
        else{

            utils::computePairWiseDtw<double>(
                    [Res](const DoubleTs &ts1, const DoubleTs &ts2) {
                        return dtw::sparse_dtw<double>(ts1, ts2, getDoubleTsElemDist, Res);
                    },
                    tss_d,
                    output_path);
        }
        
    } else if (algorithm == "pruned_dtw") {

       // int window = -1;

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
