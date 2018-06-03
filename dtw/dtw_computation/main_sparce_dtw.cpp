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
#include "sparce_dtw.hpp"
#include "sparce_dtw.cpp"


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

    //std::vector<SpeechTs> tss = utils::readChromaTs(data_file);
    std::vector<DoubleTs> tss = utils::read_UCR_TS_Archive_2015(data_path.c_str());

    //read_UCR_TS_Archive_2015
    utils::print("After!");

    //double dtw_val2 = dtw::sparce_dtw<double>(tss[0], tss[1], getSpeechTsElemDist, 0.5).dtw;
    DtwAnswer dtw1= dtw::sparce_dtw<double>(tss[2], tss[3], getDoubleTsElemDist, 0.5);
    utils::print(dtw1.dtw);
    int n1=0;
    for (auto point:dtw1.path){
       // printf("%d %d\n",point.i,point.j);
        n1+=1;
    }
    int n2=0;
    DtwAnswer dtw2 = dtw::dtw<double>(tss[2], tss[3], getDoubleTsElemDist);
    utils::print(dtw2.dtw);
    for (auto point:dtw2.path){
       // printf("%d %d\n",point.i,point.j);
        n2+=1;
    }
    printf("%d %d\n",n1,n2);
//std::list<CentParam> cent_params;
  //  cent_params.push_back({41, 10});
    //cent_params.push_back({121, 30});
    //cent_params.push_back({271, 90});
    //double dtw_val2 = dtw::msDtw(tss[0], tss[1], 30, cent_params).dtw;
    //utils::print(dtw_val2);

    //if (algorithm == "dtw") {
     //   double sec = utils::compute_cpu_time(dtw::dtw, tss);
       // std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
    //} else if (algorithm == "pruned_dtw") {
      //  double sec = utils::compute_cpu_time(dtw::sparce_dtw, tss);
        //std::cout << std::fixed << std::setprecision(2) << sec << std::endl;
   // } else {
     //   std::cout << "Algorithm is not implemented yet... :(";
    //}
    return 0;
}