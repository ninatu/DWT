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


namespace detail {
    double getElem(const std::unordered_map<DtwPathElement, DtwMartixElement> &dtw_matr, const DtwPathElement &point);

    Window expandResWindow(const Path &low_res_path, int len1, int len2, int radius, int upsample);

    SpeechTsElem quantizeFeatureVector(const SpeechTsElem &vec);

    SpeechTs quantizeFeatures(const SpeechTs &ts);

    void normalizeFeatureVector(SpeechTsElem &vec);

    SpeechTs computeCENT(const SpeechTs &ts, int window, int downsample);
}

namespace dtw {

    DoubleTs reduceDoubleTs(const DoubleTs &ts, int downsample_scale);

    SpeechTs reduceSpeechTs(const SpeechTs &ts, int w, int downsample);

    DtwAnswer msDtw(const SpeechTs &ts1, const SpeechTs &ts2, int radius, std::list<CentParam> cens_params);


    template<typename T>
    DtwAnswer dtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            const Window &window,
            std::function<double(const T&, const T&)> dist
    ) {
        DtwMatrix dtw_matr(ts1.size() + 1);
        for (int i = 0; i < ts1.size() + 1; i++) {
            dtw_matr[i] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
        }
        dtw_matr[0][0] = DtwMartixElement{0, 0, 0};
        for (auto point: window) {
            double cost = dist(ts1[point.i], ts2[point.j]);
            assignNextDtwMatrixElement(dtw_matr, point.i + 1, point.j + 1, cost);
        }
        double dtw_dist = dtw_matr[ts1.size()][ts2.size()].val;
        Path path = untwistDtwMatrixPath(dtw_matr);
        return DtwAnswer{dtw_dist, path};
    }

    template<typename T>
    DtwAnswer fastDtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            int radius,
            int downsample_scale,
            std::function<std::vector<T>(std::vector<T>, int)> reduce_ts,
            std::function<double(const T&, const T&)> dist
    ) {
        if (radius < 1) {
            throw std::exception();
        }

        int min_ts_size = radius + 2;
        if (ts1.size() < min_ts_size or ts2.size() < min_ts_size) {
            return dtw::dtw(ts1, ts2, dist);
        }

        auto shrunk_ts1 = reduce_ts(ts1, downsample_scale);
        auto shrunk_ts2 = reduce_ts(ts2, downsample_scale);
        auto low_res_path = fastDtw(shrunk_ts1, shrunk_ts2, radius, downsample_scale, reduce_ts, dist).path;

        auto window = detail::expandResWindow(low_res_path, int(ts1.size()), int(ts2.size()), radius, downsample_scale);
        return dtw(ts1, ts2, window, dist);
    }

}

