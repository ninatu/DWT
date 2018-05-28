#pragma once

#include <functional>
#include <vector>

constexpr double INF = 1e20;
constexpr double W_ij = 1;
constexpr double W_i = 1;
constexpr double W_j = 1;

struct DtwMartixElement {
    double val;
    int i;
    int j;
};

using DtwMatrix = std::vector<std::vector<DtwMartixElement>>;

struct DtwPathElement {
    int i;
    int j;
};

using Path = std::vector<DtwPathElement>;

using Window = std::vector<DtwPathElement>;

using SpeechTs = std::vector<std::vector<double>>;

using DoubleTs = std::vector<double>;

struct DtwAnswer {
    double dtw;
    Path path;
};

void assignNextDtwMatrixElement(DtwMatrix &dtw_matr, int i, int j, double cost);

Path untwistDtwMatrixPath(const DtwMatrix &dtw_matr);

bool operator==(const DtwPathElement &x, const DtwPathElement &y);

namespace std {
    template<>
    struct hash<DtwPathElement> {
        std::size_t operator()(const DtwPathElement &p) const {
            return std::hash<int>()(p.i) + std::hash<int>()(p.j);
        }
    };
}

double getDist(double x, double y);

