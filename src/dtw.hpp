#pragma once

#include <vector>
#include <algorithm>

#include "common.hpp"


namespace dtw {
    double dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws = -1);

    template<typename T>
    DtwAnswer dtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            std::function<double(T, T)> dist,
            int window = -1
    ) {
        if (window < 0) {
            window = int(ts2.size());
        }
        DtwMatrix dtw_matr(ts1.size() + 1);
        dtw_matr[0] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
        dtw_matr[0][0] = DtwMartixElement{0, 0, 0};

        for (int i = 1; i <= ts1.size(); i++) {
            dtw_matr[i] = std::vector<DtwMartixElement>(ts2.size() + 1, {INF, 0, 0});
            for (int j = std::max(1, i - window); j <= std::min<int>(ts2.size(), i + window); j++) {
                double cost = dist(ts1[i - 1], ts2[j - 1]);
                assignNextDtwMatrixElement(dtw_matr, i, j, cost);
            }
        }
        double dtw_dist = dtw_matr[ts1.size()][ts2.size()].val;
        Path path = untwistDtwMatrixPath(dtw_matr);
        return DtwAnswer{dtw_dist, path};
    }
}
