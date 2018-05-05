#include "utils.hpp"
#include "dtw.hpp"


double dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws) {
    int n = ts1.size() - 1;
    int m = ts2.size() - 1;

    if (ws == 0) {
        ws = m;
    }

    // contain only two vectors for odd and even rows
    std::vector<std::vector<double>> dist(2);
    dist[0] = std::vector<double>(m + 1, INF);
    dist[1] = std::vector<double>(m + 1, INF);
    dist[0][0] = 0;

    for (int i = 1; i <= n; i++) {
        dist[i % 2][0] = INF;
        for (int j = std::max<int>(1, i - ws); j <= std::min<int>(m, i + ws); j++) {
            dist[i % 2][j] = cost(ts1[i], ts2[j]) +
                             std::min(std::min(dist[(i - 1) % 2][j], dist[i % 2][j - 1]), dist[(i - 1) % 2][j - 1]);
        }
    }
    return dist[n % 2][m];
}