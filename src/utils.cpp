#include "utils.hpp"


namespace utils {
    std::vector<DoubleTs> read_UCR_TS_Archive_2015(const std::string &path) {
        std::ifstream data_file(path.c_str());
        std::vector<DoubleTs> tss;

        if (data_file.is_open()) {
            std::string line;
            double x;
            char c;
            while (std::getline(data_file, line)) {
                DoubleTs ts;
                std::stringstream str_line(line);
                // read first number, it is number of cluster
                str_line >> x;

                // read ts
                while (str_line >> c >> x) {
                    ts.push_back(x);
                }
                tss.push_back(ts);
            }
        }
        return tss;
    }


//            Data format:
//            [[0.7811984  0.36267271 0.49415439 ... 0.24230753 0.51290877 0.51705223]
//            [0.37969591 0.61288756 0.86381055 ... 0.54354581 0.68339427 0.43217374]
//            [0.30791091 0.90021918 1.         ... 0.48684803 0.57208837 0.22222787]
//            ...
//            [0.4602146  1.         0.5061302  ... 0.35703402 0.36713959 0.49867281]
//            [0.39315954 0.97369524 0.34206838 ... 0.3624009  0.31719481 1.        ]
//            [0.6515769  0.42988866 0.3856414  ... 0.29266979 0.47215005 0.50335449]]
    std::vector<SpeechTs> readChromaTs(std::istream &file) {
        std::vector<SpeechTs> tss;
        std::string line;
        double x;
        char c;

        while (!file.eof()) {
            std::getline(file, line);
            std::stringstream str_line(line);
            // read [
            str_line >> c;
            if (c != '[') {
                utils::print("\'[\' of ts is not found ");
                throw std::exception();
            }
            //read ts
            SpeechTs ts;
            for (;;) {
                // read [
                str_line >> c;
                if (c != '[') {
                    utils::print("\'[\' of chroma vector is not found ");
                    throw std::exception();
                }
                // read chroma vector
                SpeechTsElem vec;
                for (int i = 0; i < 12; i++) {
                    str_line >> x;
                    vec.push_back(x);
                }
                // read ]
                str_line >> c;
                if (c != ']') {
                    utils::print(" \']\' of chroma vector is not found");
                    throw std::exception();
                }
//                print("Chroma vector is readed");
                ts.push_back(vec);
                str_line >> c;
                if (str_line.eof()) {
                    std::getline(file, line);
                    str_line = std::stringstream (line);
                } else if (c == ']') {
                    break;
                } else {
                    utils::print("\']\' of ts is not found ");
                    throw std::exception();
                }

            }
            tss.push_back(ts);
//            print("TS is readed");
        }
        return tss;
    }

    void printComandLineErrorMessage(int argc, char **argv) {
        std::cerr << "Invalid command line parameters: ";
        for (int i = 0; i < argc; i++) {
            std::cerr << argv[i] << " ";
        }
        std::cerr << std::endl;
        std::cerr << "Usage: " << argv[0] << " algorith(dtw|pruned_dtw|fast_dtw) part_to_input_file" << std::endl;
    }


    int argmin(double x0, double x1, double x2) {
        if (x0 <= x1) {
            if (x0 <= x2)
                return 0;
            else
                return 2;
        } else {
            if (x1 <= x2)
                return 1;
            else
                return 2;
        }
    }

}
