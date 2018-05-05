#include "pruned_dtw.hpp"
#include "utils.hpp"


double pruned_dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws) {
    int n = ts1.size() - 1;
    int m = ts2.size() - 1;

    if (ws == 0) {
        ws = m;
    }

    //create UB partial
    std::vector<double> ub_partial(n + 1);
    double last_ub = 0;
    if (n < m) {
        for(int i = n + 1; i <= m; i++) {
            last_ub += cost(ts1.at(n), ts2.at(i));
        }
    }
    for(int i = n; i >= 0; i--) {
        ub_partial.at(i) = last_ub;
        last_ub += cost(ts1.at(i), ts2.at(std::min(i, m)));
    }
    double ub = ub_partial[0];

    // contain only two vectors for odd and even rows
    std::vector<std::vector<double>> dist(2);
    dist[0] = std::vector<double>(m + 1, INF);
    dist[1] = std::vector<double>(m + 1, INF);
    dist[0][0] = 0;

    int sc = 1;
    int ec = 1;
    for (int i = 1; i <= n; i++) {
        bool smaler_found = false;
        int ec_next = i;
        dist[i % 2][sc - 1] = INF;
        for(int j = std::max<int>(sc, i - ws); j <= std::min<int>(m, i + ws); j++) {
            if (j <= ec) {
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
        ec = ec_next;
        ub = dist[i % 2][i] + ub_partial[i];
    }
    return dist[n % 2][m];
}
