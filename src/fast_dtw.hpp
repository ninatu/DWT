#pragma once

#include <vector>
#include <functional>
#include <exception>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "utils.hpp"
#include "common.hpp"
#include "dtw.hpp"


namespace detail {
    double getElem(const std::unordered_map<DtwPathElement, DtwMartixElement> &dtw_matr, const DtwPathElement &point);

    Window expandResWindow(const Path low_res_path, int len1, int len2, int radius, int upsample);
}

namespace dtw {

    DoubleTs reduce_double_ts(const DoubleTs &ts, int downsample_scale);

    SpeechTs reduce_speech_ts(const SpeechTs &ts, int w, int downsample);

    template<typename T>
    DtwAnswer dtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            const Window &window,
            std::function<double(T, T)> dist
    ) {
        std::unordered_map<DtwPathElement, DtwMartixElement> dtw_matr;
        dtw_matr[DtwPathElement{0, 0}] = DtwMartixElement{0, 0, 0};

        for (auto point: window) {
            double cost = dist(ts1[point.i], ts2[point.j]);
            int i = point.i + 1;
            int j = point.j + 1;
            double d_ij = detail::getElem(dtw_matr, DtwPathElement{i - 1, j - 1}) + W_ij * cost;
            double d_i = detail::getElem(dtw_matr, DtwPathElement{i - 1, j}) + W_i * cost;
            double d_j = detail::getElem(dtw_matr, DtwPathElement{i, j - 1}) + W_j * cost;
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
        double dtw_dist = dtw_matr[DtwPathElement{int(ts1.size()), int(ts2.size())}].val;

        std::vector<DtwPathElement> path;
        DtwPathElement path_elem{int(ts1.size()), int(ts2.size())};
        while (path_elem.i != 0 or path_elem.j != 0) {
            path.push_back(DtwPathElement{path_elem.i - 1, path_elem.j - 1});
            DtwMartixElement cut_matr_elem = dtw_matr[path_elem];
            path_elem = DtwPathElement{cut_matr_elem.i, cut_matr_elem.j};
        }
        std::reverse(path.begin(), path.end());
        return DtwAnswer{dtw_dist, path};
    }

    template<typename T>
    DtwAnswer fastDtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            int radius,
            int downsample_scale,
            std::function<std::vector<T>(std::vector<T>, int)> reduce_ts,
            std::function<double(T, T)> dist
    ) {
        int min_ts_size = radius + 2;
        if (ts1.size() < min_ts_size or ts2.size() < min_ts_size) {
            return dtw(ts1, ts2, dist);
        }

        auto shrunk_ts1 = reduce_ts(ts1, downsample_scale);
        auto shrunk_ts2 = reduce_ts(ts2, downsample_scale);
        auto low_res_path = fastDtw(shrunk_ts1, shrunk_ts2, radius, downsample_scale, reduce_ts, dist).path;

        auto window = detail::expandResWindow(low_res_path, int(ts1.size()), int(ts2.size()), radius, downsample_scale);
        return dtw(ts1, ts2, window, dist);
    }
}

