#include <cmath>

#include "common.hpp"
#include "fast_dtw.hpp"


namespace detail {
    double getElem(const std::unordered_map<DtwPathElement, DtwMartixElement> &dtw_matr, const DtwPathElement &point) {
        auto it = dtw_matr.find(point);
        if (it != dtw_matr.end()) {
            return it->second.val;
        }
        return INF;
    }

    Window expandResWindow(const Path &low_res_path, int len1, int len2, int radius, int upsample) {
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

    SpeechTsElem quantizeFeatureVector(const SpeechTsElem &vec) {
        SpeechTsElem quant_vec(vec.size());
        for (auto elem : vec) {
            if (elem >= 0.4) {
                quant_vec.push_back(4);
            } else if (elem >= 0.2) {
                quant_vec.push_back(3);
            } else if (elem >= 0.1) {
                quant_vec.push_back(2);
            } else if (elem >= 0.05) {
                quant_vec.push_back(1);
            } else {
                quant_vec.push_back(0);
            }
        }
        return quant_vec;
    }

    SpeechTs quantizeFeatures(const SpeechTs &ts) {
        SpeechTs quantTs;
        for (auto ts_elem : ts) {
            quantTs.push_back(detail::quantizeFeatureVector(ts_elem));
        }

    }

    void normalizeFeatureVector(SpeechTsElem &vec) {
        double sum = 0;
        for (auto elem : vec) {
            sum += elem * elem;
        }
        for (auto &elem : vec) {
            elem /= sum;
        }
    }

    SpeechTs computeCENT(const SpeechTs &ts, int window, int downsample) {
        SpeechTs quant_ts = quantizeFeatures(ts);

        std::vector<double> hann_window(window);
        for (int i = 0; i < window; i++) {
            hann_window[i] = (1 - std::cos(2 * PI * i / (window - 1))) / 2;
        }

        int cent_size = (int) floor((float) quant_ts.size() / (float) downsample);
        int half_window = (int) floor((float) window / 2.0);
        int feature_count = quant_ts[0].size();

        SpeechTs cent(cent_size);
        for (int i = 0; i < cent_size; i++) {
            cent[i] = std::vector<double>(feature_count, 0);

            for (int j = 0; j < window; j++) {
                int n = i * downsample + j - half_window;
                if (n >= 0 and n < quant_ts.size()) {
                    const std::vector<double> &features = quant_ts[n];
                    for (int k = 0; k < feature_count; k++) {
                        cent[i][k] += hann_window[j] * features[k];
                    }
                }
            }
            normalizeFeatureVector(cent[i]);
        }
        return cent;
    }
}

namespace dtw {
    DoubleTs reduceDoubleTs(const DoubleTs &ts, int downsample_scale) {
        DoubleTs reduced_ts;
        double sum = 0;
        int count = 0;
        for (auto elem: ts) {
            sum += elem;
            count += 1;
            if (count == downsample_scale) {
                reduced_ts.push_back(sum / downsample_scale);
                sum = 0;
                count = 0;
            }
        }
        return reduced_ts;
    }

    SpeechTs reduceSpeechTs(const SpeechTs &ts, int w, int downsample) {
        return ts;
    }

    DtwAnswer msDtw(
            const SpeechTs &ts1,
            const SpeechTs &ts2,
            int radius,
            std::list<CentParam> cens_params
    ) {
        if (radius < 1) {
            throw std::exception();
        }

        int min_ts_size = radius + 2;
        if (ts1.size() < min_ts_size or ts2.size() < min_ts_size) {
            return dtw<SpeechTsElem>(ts1, ts2, getSpeechTsElemDist);
        }

        int feature_window = cens_params.front().window;
        int downsample = cens_params.front().downsample;

        cens_params.pop_front();

        auto shrunk_ts1 = detail::computeCENT(ts1, feature_window, downsample);
        auto shrunk_ts2 = detail::computeCENT(ts2, feature_window, downsample);

        auto low_res_path = msDtw(shrunk_ts1, shrunk_ts2, radius, cens_params).path;

        auto window = detail::expandResWindow(low_res_path, int(ts1.size()), int(ts2.size()), radius, downsample);
        return dtw<SpeechTsElem>(ts1, ts2, window, getSpeechTsElemDist);
    }
}

