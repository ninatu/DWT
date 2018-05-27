#include <vector>
#include <functional>
#include <exception>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "utils.hpp"

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


struct DtwMartixElement{
    double val;
    int i;
    int j;
};


struct DtwPathElement{
    int i;
    int j;
};

bool operator == (const DtwPathElement &x, const DtwPathElement &y) {
    return (x.i == y.i) and (x.j == y.j);
}

namespace std {
    template<>
    struct hash<DtwPathElement> {
        std::size_t operator()(const DtwPathElement& p) const {
            return std::hash<int>()(p.i) + std::hash<int>()(p.j);
        }
    };
}


using Path = std::vector<DtwPathElement>;

using Window = std::vector<DtwPathElement>;

using SpeechTs = std::vector<std::vector<double>>;

struct DtwAnswer {
    double dtw;
    Path path;
};


template <typename T>
DtwAnswer dtw(const std::vector<T> &ts1, const std::vector<T> &ts2, std::function<double(T, T)> dist) {
    std::vector<std::vector<DtwMartixElement>> dtw_matr;
    dtw_matr[0] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
    dtw_matr[0][0] = DtwMartixElement{0, 0, 0};

    for (int i = 1; i <= ts1.size(); i++) {
        dtw_matr[i] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
        for (int j = 1; j <= ts2.size(); ++j) {
            double cost = dist(ts1[i], ts2[j]);
            double d_ij = dtw_matr[i - 1][j - 1].val + W_ij * cost;
            double d_i = dtw_matr[i - 1][j].val + W_i * cost;
            double d_j = dtw_matr[i][j - 1].val + W_j * cost;

            switch (argmin(d_ij, d_i, d_j)) {
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
    }
    double dtw_dist = dtw_matr[ts1.size()][ts2.size()].val;
    std::vector<DtwPathElement> path;
    DtwPathElement path_elem{int(ts1.size()), int(ts2.size())};
    while(path_elem.i != 0 or path_elem.j != 0) {
        path.push_back(DtwPathElement{path_elem.i - 1, path_elem.j - 1});
        DtwMartixElement cut_matr_elem = dtw_matr[path_elem.i][path_elem.j];
        path_elem = DtwPathElement{cut_matr_elem.i, cut_matr_elem.j};
    }
    std::reverse(path.begin(), path.end());
    return DtwAnswer{dtw_dist, path};
}



double get_elem(const std::unordered_map<DtwPathElement, DtwMartixElement> &dtw_matr, const DtwPathElement &point) {
    auto it = dtw_matr.find(point);
    if (it != dtw_matr.end()) {
        return it->second.val;
    }
    return INF;
}


template <typename T>
DtwAnswer dtw(const std::vector<T> &ts1, const std::vector<T> &ts2, const Window &window, std::function<double(T, T)> dist) {
    std::unordered_map<DtwPathElement, DtwMartixElement> dtw_matr;
    dtw_matr[DtwPathElement{0, 0}] = DtwMartixElement{0, 0, 0};

    for (auto point: window) {
        double cost = dist(ts1[point.i], ts2[point.j]);
        int i = point.i + 1;
        int j = point.j + 1;
        double d_ij = get_elem(dtw_matr, DtwPathElement{i - 1, j - 1}) + W_ij * cost;
        double d_i = get_elem(dtw_matr, DtwPathElement{i - 1, j}) + W_i * cost;
        double d_j = get_elem(dtw_matr, DtwPathElement{i, j - 1})+ W_j * cost;
        switch (argmin(d_ij, d_i, d_j)) {
            case 0:
                dtw_matr[DtwPathElement{i, j}] = DtwMartixElement{d_ij, i - 1, j - 1};
                break;
            case 1:
                dtw_matr[DtwPathElement{i, j}] = DtwMartixElement{d_i, i - 1, j};
                break;
            case 2:
                dtw_matr[DtwPathElement{i, j}] = DtwMartixElement{d_j, i, j - 1};
                break;
            default:
                throw std::exception();
        }
    }
    double dtw_dist = dtw_matr[DtwPathElement{ts1.size(), ts2.size()}].val;

    std::vector<DtwPathElement> path;
    DtwPathElement path_elem{int(ts1.size()), int(ts2.size())};
    while(path_elem.i != 0 or path_elem.j != 0) {
        path.push_back(DtwPathElement{path_elem.i - 1, path_elem.j - 1});
        DtwMartixElement cut_matr_elem = dtw_matr[path_elem];
        path_elem = DtwPathElement{cut_matr_elem.i, cut_matr_elem.j};
    }
    std::reverse(path.begin(), path.end());
    return DtwAnswer{dtw_dist, path};
}


Window expand_res_window(const Path low_res_path, int len1, int len2, int radius, int upsample) {
    std::unordered_set<DtwPathElement> low_res_expand_path;
    for (auto point: low_res_path) {
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                low_res_expand_path.insert({point.i + i, point.j + j});
            }
        }
    }

    std::unordered_set<DtwPathElement> high_res_expand_path;
    for (auto point : low_res_expand_path) {
        for (int i = 0; i < upsample; i++) {
            for (int j = 0; j < upsample; j++) {
                high_res_expand_path.insert({point.i * upsample + i, point.j * upsample + j});
            }
        }
    }

    Window window;
    int start_j = 0;
    for (int i = 0; i < len1; i++) {
        int new_start_j = -1;
        for (int j = start_j; j < len2; j++) {
            DtwPathElement cur_point{i, j};
            if (high_res_expand_path.find(cur_point) != high_res_expand_path.end()) {
                window.push_back(cur_point);
                if (new_start_j < 0) {
                    new_start_j = start_j;
                }
            } else if (new_start_j >= 0) {
                break;
            }
        }
        start_j = new_start_j;
    }
    return window;
}

template <typename T>
DtwAnswer fast_dtw(
        const std::vector<T> &ts1,
        const std::vector<T> &ts2,
        int radius,
        int downsample,
        std::function<std::vector<T>(std::vector<T>)> reduce_ts,
        std::function<double(T, T)> dist
    )
{
    int min_ts_size = radius + 2;
    if (ts1.size() < min_ts_size or ts2.size() < min_ts_size) {
        return dtw(ts1, ts2, dist);
    }

    auto shrunk_ts1 = reduce_ts(ts1);
    auto shrunk_ts2 = reduce_ts(ts2);
    auto low_res_path = fast_dtw(shrunk_ts1, shrunk_ts2, radius, downsample, reduce_ts, dist).path;

    auto window = expand_res_window(low_res_path, int(ts1.size()), int(ts2.size()), radius, downsample);
    return dtw(ts1, ts2, window, dist);
}



SpeechTs reduce_speech_ts(const SpeechTs &ts, int w, int downsample) {
    return ts;
}
