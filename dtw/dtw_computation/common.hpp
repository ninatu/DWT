#pragma once

#include <functional>
#include <vector>

constexpr double INF = 1e20;
constexpr double W_ij = 1;
constexpr double W_i = 1;
constexpr double W_j = 1;
constexpr double PI  = 3.141592653589793238463;


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

using SpeechTsElem = std::vector<double>;
using SpeechTs = std::vector<SpeechTsElem>;

using DoubleTs = std::vector<double>;

struct DtwAnswer {
    double dtw;
    Path path;
};

struct CentParam {
    int window;
    int downsample;
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

double getDoubleTsElemDist(const double &x, const double &y);

double getSpeechTsElemDist(const SpeechTsElem &x, const SpeechTsElem &y);
