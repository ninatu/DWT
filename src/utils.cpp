#include "utils.hpp"

std::vector<std::vector<double>> readInputFile(const std::string &path) {
    std::ifstream data_file(path.c_str());
    std::vector<std::vector<double>> tss;

    if (data_file.is_open()) {
        std::string line;
        double x;
        char c;
        while (std::getline(data_file, line)) {
            std::vector<double> ts;
            std::stringstream str_line(line);
            // read first number, it is number of cluster
            str_line >> x;

            // push first empty element for easy computation
//            ts.push_back(0);

            // read ts
            while(str_line >> c >> x) {
                ts.push_back(x);
            }
            tss.push_back(ts);
        }
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
