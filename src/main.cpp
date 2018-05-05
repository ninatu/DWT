#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <exception>

#define INF 1e20

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
            ts.push_back(0);

            // read ts
            while(str_line >> c >> x) {
                ts.push_back(x);
            }
            tss.push_back(ts);
        }
    }
    return tss;
}

//std::vector<std::vector<double>> dtw(const std::vector<std::vector<double>> &tss) {
//    std::vector<std::vector<double>> dist;
//
//}

double cost(double x, double y) {
    return (x - y) * (x - y);
}

double dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws=0) {
    int n = ts1.size() - 1;
    int m = ts2.size() - 1;

    if (ws == 0) {
        ws = m;
    }

    // contain only two vectors for odd and even rows
    std::vector<std::vector<double>> dist(2);
    dist[0] = std::vector<double> (m + 1, INF);
    dist[1] = std::vector<double> (m + 1, INF);
    dist[0][0] = 0;

    for(int i = 1; i <= n; i++) {
//        for(int j = std::max<int>(1, i - ws); j <= std::min<int>(m, i + ws); j++) {
        for(int j = 1; j <= m; j++) {
            dist[i % 2][j] = cost(ts1[i], ts2[j]) +
                             std::min(std::min(dist[(i - 1) % 2][j], dist[i % 2][j - 1]), dist[(i - 1) % 2][j - 1]);
        }
    }
    return dist[n % 2][m];
}


double pruned_dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws=0) {
    int n = ts1.size() - 1;
    int m = ts2.size() - 1;

    if (ws == 0) {
        ws = m;
    }

    //create UB partial
//    std::vector<double> ub_partial(n + 1);
//    double last_ub = 0;
//    if (n < m) {
//        for(int i = n + 1; i <= m; i++) {
//            last_ub += cost(ts1.at(n), ts2.at(i));
//        }
//    }
//    for(int i = n; i >= 0; i--) {
//        ub_partial.at(i) = last_ub;
//        last_ub += cost(ts1.at(i), ts2.at(std::min(i, m)));
//    }
//    double ub = ub_partial[0];

    std::vector<double> ub_partial(n + 1);
    double last_ub = 0;
    for(int i = n; i >= 0; i--) {
        ub_partial.at(i) = last_ub;
        last_ub += cost(ts1.at(i), ts2.at(i));
    }

    double ub = ub_partial[0];
    std::cout << ub << std::endl;
//    if (n == m) {
//        double tmp = 0;
//        for(int i = 1; i <= n; i++){
//            tmp += cost(ts1[i], ts2[i]);
//        }
//        if (std::abs(tmp - last_ub) > 1e-6) {
//            std::cout << tmp << " " << last_ub;
//            throw  std::exception();
//        }
//    }

    // contain only two vectors for odd and even rows
    std::vector<std::vector<double>> dist(2);
    dist[0] = std::vector<double> (m + 1, INF);
    dist[1] = std::vector<double> (m + 1, INF);
    dist[0][0] = 0;

    int sc = 1;
    int ec = 1;
    double prev_ec = 1;

    for(int i = 1; i <= n; i++) {
        bool smaler_found = false;
        int ec_next = i;
        dist[i % 2][sc - 1] = INF;
//        for(int j = std::max<int>(sc, i - ws); j <= std::min<int>(m, i + ws); j++) {
        for(int j = sc; j <= m; j++) {
            if (j <= prev_ec) {
                dist[i % 2][j] = cost(ts1[i], ts2[j]) +
                                 std::min(std::min(dist[(i - 1) % 2][j], dist[i % 2][j - 1]), dist[(i - 1) % 2][j - 1]);
            } else {
                dist[i % 2][j] = cost(ts1[i], ts2[j]) + dist[i % 2][j - 1];
            }

            // prunning
            if (dist[i % 2][j] > ub) {
                if (!smaler_found) {
                    sc = j + 1;
                }
                if (j >= ec) {
                    break;
                }
            } else {
                smaler_found = true;
                ec_next = j + 1;
            }
        }
        prev_ec = ec;
        ec = ec_next;
//        ub = dist[i % 2][i] + ub_partial[i];
    }
    return dist[n % 2][m];
}


void printComandLineErrorMessage(int argc, char **argv) {
    std::cerr << "Invalid command line parameters: ";
    for(int i = 0; i < argc; i++) {
        std::cerr << argv[i] << " ";
    }
    std::cerr << std::endl;
    std::cerr << "Usage: " << argv[0] << " algorith(dtw|pruned_dtw|fast_dtw) part_to_input_file" << std::endl;
}

int main(int argc, char *argv[]) {
    std::string algorithm;
    std::string data_path;
    if (argc == 3) {
        algorithm = argv[1];
        data_path = argv[2];
        if (algorithm != "dtw" and algorithm != "pruned_dtw" and algorithm != "fast_dtw") {
            printComandLineErrorMessage(argc, argv);
            return -1;
        }
    } else {
        printComandLineErrorMessage(argc, argv);
        return -1;
    }

    std::vector<std::vector<double>> tss = readInputFile(data_path);
    if (algorithm == "dtw") {
        int i = 0, j = 1;
        std::cout << tss.at(i).size() << " " <<  tss.at(j).size() << std::endl;
        std::cout << "DTW:" << dtw(tss.at(i), tss.at(j), 300) << std::endl;
        std::cout << "Pruned DTW:" << pruned_dtw(tss.at(i), tss.at(j), 300) << std::endl;
//        for(int i = 0; i < tss.size(); i++) {
//            for(int j = i + 1; j < tss.size(); j++) {
//                double x = dtw(tss.at(i), tss.at(j));
//                double y =  pruned_dtw(tss.at(i), tss.at(j));
//                if (std::abs(x - y) > 1e-6) {
//                    std::cout << i << " " << j << std::endl;
//                    std::cout << "DTW:" <<  x << std::endl;
//                    std::cout << "Pruned DTW:" << y << std::endl;
////                    throw std::exception();
//                }
//            }
//        }

    } else if (algorithm == "pruned_dtw") {
        std::cout << "Pruned DTW:" << pruned_dtw(tss.at(0), tss.at(1)) << std::endl;
    } else {
        std::cout << "Algorithm is not implemented yet... :(";
    }
    return 0;
}