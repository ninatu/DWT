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

    Window expandResWindow(const Path low_res_path, int len1, int len2, int radius, int upsample) {
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
}

namespace dtw {
    DoubleTs reduce_double_ts(const DoubleTs &ts, int downsample_scale) {
        DoubleTs reduced_ts;
        double sum = 0;
        int count = 0;
        for(auto elem: ts) {
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

    SpeechTs reduce_speech_ts(const SpeechTs &ts, int w, int downsample) {
        return ts;
    }
}

