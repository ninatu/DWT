#pragma once

#include <vector>
#include <list>
#include <functional>
#include <exception>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "utils.hpp"
#include "common.hpp"
#include "dtw.hpp"

struct DtwBin {
    double i;
    double j;
};

using Bins = std::vector<DtwBin>;

using PointsVector = std::vector<DtwPathElement>;

