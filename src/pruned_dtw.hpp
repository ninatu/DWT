#pragma once

#include <vector>

#include "common.hpp"
#include "utils.hpp"


namespace dtw {
    template<typename T>
    DtwAnswer prunedDtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            std::function<double(T, T)> dist,
            int window = -1
    ) {
        int n = int(ts1.size());
        int m = int(ts2.size());

        if (window == -1) {
            window = m;
        }

        //create UB partial
        std::vector<double> ub_partial(n + 1);
        double last_ub = 0;
        if (n < m) {
            for (int i = n + 1; i <= m; i++) {
                last_ub += getDist(ts1[n - 1], ts2[i - 1]);
            }
        }
        for (int i = n; i > 0; i--) {
            ub_partial[i] = last_ub;
            last_ub += getDist(ts1[i - 1], ts2[std::min(i, m) - 1]);
        }
        ub_partial[0] = last_ub;
        double ub = ub_partial[0];

        // contain only two vectors for odd and even rows
        std::vector<std::vector<DtwMartixElement>> dtw_matr(n + 1);
        dtw_matr[0] = std::vector<DtwMartixElement>(m + 1, {INF, 0, 0});
        dtw_matr[0][0] = DtwMartixElement{0, 0, 0};

        int sc = 1;
        int ec = 1;
        for (int i = 1; i <= n; i++) {
            bool smaler_found = false;
            int ec_next = i;
            dtw_matr[i] = std::vector<DtwMartixElement>(m + 1, {INF, 0, 0});
            for (int j = std::max<int>(sc, i - window); j <= std::min<int>(m, i + window); j++) {
                double cost = dist(ts1[i - 1], ts2[j - 1]);
                assignNextDtwMatrixElement(dtw_matr, i, j, cost);

                // prunning
                if (dtw_matr[i][j].val > ub) {
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
            ec = ec_next;
            ub = dtw_matr[i][i].val + ub_partial[i];
        }
        return DtwAnswer{dtw_matr[n][m].val, untwistDtwMatrixPath(dtw_matr)};
    }
}