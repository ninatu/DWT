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
            if (str_line.eof()) {
                break;
            }
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
