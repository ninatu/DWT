#include <algorithm>
#include <exception>

#include "common.hpp"
#include "utils.hpp"


bool operator==(const DtwPathElement &x, const DtwPathElement &y) {
    return (x.i == y.i) and (x.j == y.j);
}

double getDoubleTsElemDist(const double &x, const double &y) {
    return (x - y) * (x - y);
}

double getSpeechTsElemDist(const SpeechTsElem &x, const SpeechTsElem &y) {
    if (x.size() != y.size()) {
        throw std::exception();
    }

    double sum = 0;
    for (int i = 0; i < x.size(); i ++) {
        sum += x[i] * y[i];
    }
    return sum;
}

void assignNextDtwMatrixElement(DtwMatrix &dtw_matr, int i, int j, double cost) {
    double d_ij = dtw_matr[i - 1][j - 1].val + W_ij * cost;
    double d_i = dtw_matr[i - 1][j].val + W_i * cost;
    double d_j = dtw_matr[i][j - 1].val + W_j * cost;

    switch (utils::argmin(d_ij, d_i, d_j)) {
        case 0:
            dtw_matr[i][j] = DtwMartixElement{d_ij, i - 1, j - 1};
            break;
        case 1:
            dtw_matr[i][j] = DtwMartixElement{d_i, i - 1, j};
            break;
        case 2:
            dtw_matr[i][j] = DtwMartixElement{d_j, i, j - 1};
            break;
        default:
            throw std::exception();
    }
}

Path untwistDtwMatrixPath(const DtwMatrix &dtw_matr) {
    Path path;
    DtwPathElement path_elem{int(dtw_matr.size()) - 1, int(dtw_matr.at(0).size()) - 1};
    while (path_elem.i != 0 or path_elem.j != 0) {
        path.push_back(DtwPathElement{path_elem.i - 1, path_elem.j - 1});
        DtwMartixElement cut_matr_elem = dtw_matr[path_elem.i][path_elem.j];
        path_elem = DtwPathElement{cut_matr_elem.i, cut_matr_elem.j};
    }
    std::reverse(path.begin(), path.end());
    return path;
}

