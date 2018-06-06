#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "common.hpp"


namespace utils {

    std::vector<DoubleTs> read_UCR_TS_Archive_2015(const std::string &path);

//            SpeechTs data format:
//            [[0.7811984  0.36267271 0.49415439 ... 0.24230753 0.51290877 0.51705223]
//            ...
//            [0.6515769  0.42988866 0.3856414  ... 0.29266979 0.47215005 0.50335449]]
//            [[0.7811984  0.36267271 0.49415439 ... 0.24230753 0.51290877 0.51705223]
//            ...
//            [0.6515769  0.42988866 0.3856414  ... 0.29266979 0.47215005 0.50335449]]
    std::vector<SpeechTs> readChromaTs(const std::string &path);

    int argmin(double x0, double x1, double x2);

    template<typename Head>
    void print(Head &&h) {
        std::cout << std::forward<Head>(h) << std::endl;
    }

    template<typename Head, typename... Args>
    void print(Head &&h, Args &&... args) {
        std::cout << std::forward<Head>(h) << " ";
        print(std::forward<Args>(args)...);
    }

    template<typename T>
    double computePairWiseDtw(
            std::function<DtwAnswer(const std::vector<T> &, const std::vector<T> &)> algorithm,
            const std::vector<std::vector<T>> &tss,
            const std::string &output_path
    ) {
        std::fstream f_out(output_path, std::fstream::out);
        f_out << '{' << '\n';
        for (int i = 0; i < tss.size() - 1; i++) {
            for (int j = i + 1; j < tss.size(); j++) {
                // compute dtw
                std::clock_t c_start = std::clock();
                DtwAnswer dtw_answer = algorithm(tss[i], tss[j]);
                std::clock_t c_end = std::clock();
                double sec =  double(c_end - c_start) / (CLOCKS_PER_SEC);
                std::cout << std::fixed << std::setprecision(2) << i << ", " << j <<
                          ": dtw: " << dtw_answer.dtw << ", time: "<< sec << std::endl;

                // saving path
                f_out << '"' << i << ',' << j << '"' << ": [" ;
                for (int k = 0; k < dtw_answer.path.size(); k++) {
                    auto path_elem = dtw_answer.path[k];
                    f_out << '[' << path_elem.i << ',' << path_elem.j << ']';
                    if (k != dtw_answer.path.size() - 1) {
                        f_out << ", ";
                    }
                }
                f_out << ']';
                if (j != tss.size() - 1) {
                    f_out << ",\n";
                }
            }
            if (i != tss.size() - 2) {
                f_out << ",\n";
            }
        }
        f_out << "}";
    }

}
