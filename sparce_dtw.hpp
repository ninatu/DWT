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

struct max_min{
    double max_el;
    double min_el;
};

using Bins = std::vector<DtwBin>;

using PointsVector = std::vector<DtwPathElement>;

namespace detail{
    SpeechTs normFeatureVectorSparce(const SpeechTs &vec, double max_elem, double min_elem) {


        SpeechTs ts;

        for (auto mf : vec) {

            SpeechTsElem h;

            for (auto elem : mf) {
                h.push_back((elem - min_elem) / (max_elem - min_elem));
            }
            ts.push_back(h);
            h.clear();
        }
        return ts;
    }


    DoubleTs normFeatureVectorSparce(const DoubleTs &vec, double max_elem, double min_elem) {

        DoubleTs ts;

        for (auto elem : vec) {
            ts.push_back((elem - min_elem) / (max_elem - min_elem));
        }
        return ts;
    }


    max_min getMaxMin(const DoubleTs &vec) {
        double min_elem = INF;
        double max_elem = 0;
        //DoubleTs ts;

        for (auto elem : vec) {
            if (elem > max_elem) {
                max_elem = elem;
            }
            if (elem < min_elem) {
                min_elem = elem;
            }
            //  ts.push_back(elem);
        }

        return max_min{max_elem, min_elem};
    }

    max_min getMaxMin(const SpeechTs &vec) {


        double max_elem = 0;
        double min_elem = INF;

        for (auto mf : vec) {
            for (auto elem : mf) {
                if (elem > max_elem) {
                    max_elem = elem;
                }
                if (elem < min_elem) {
                    min_elem = elem;
                }
            }
        }
        //printf("%.9f \n",min_elem);
        //printf("%.9f \n",max_elem);
        return max_min{max_elem, min_elem};
    }

    bool checkAllign(const double &elem1, const double &elem2, double step, int n){

        //bool allign_k = false;
        // for (auto bin : bins) {
        // if (std::abs(elem1 - elem2) < step) {
        //          allign_k = true;
        //         break;
        //     }
        // }
        return std::abs(elem1 - elem2) < step;

    }

    bool checkAllign(const SpeechTsElem &elem1, const SpeechTsElem &elem2, double step, int n){

        if (elem1.size() != elem2.size()) {
            throw std::exception();
        }

        bool allign = true;
        int allign_k = 0;
        for (int k = 1; k <= elem1.size(); k++) {

            // for (auto bin : bins) {
            if (std::abs(elem1[k] - elem2[k]) >= step) {
                allign_k += 1;
                if (allign_k > n){
                    allign = false;
                    break;
                }

            }
        }


        return allign;
    }

    bool checkAllign(const double &elem1, const double &elem2, Bins &bins){

        bool allign_k = false;
        for (auto bin : bins) {
            if ((elem1 <= bin.j and elem1 >= bin.i) and (elem2 <= bin.j and elem2 >= bin.i)) {
                allign_k = true;
                break;
            }
        }
        return allign_k;

    }

    bool checkAllign(const SpeechTsElem &elem1, const SpeechTsElem &elem2, Bins &bins) {

        if (elem1.size() != elem2.size()) {
            throw std::exception();
        }

        bool allign = true;

        for (int k = 1; k <= elem1.size(); k++) {
            bool allign_k = false;
            for (auto bin : bins) {
                if ((elem1[k] <= bin.j and elem1[k] >= bin.i) and (elem2[k] <= bin.j and elem2[k] >= bin.i)) {
                    allign_k = true;
                    break;
                }
            }
            if (not allign_k) {
                allign = false;
                break;
            }
        }

        return allign;
    }

    template<typename T>
    PointsVector quantFeatureVector(const T &ts1, const T &ts2, Bins &bins, DtwMatrix &dtw_matr) {

        PointsVector alligns;


        for (int i = 0; i < ts1.size(); i++) {
            for (int j = 0; j < ts2.size(); j++) {

                if (detail::checkAllign(ts1[i], ts2[j], bins)){
                    //alligns.push_back(DtwPathElement{i,j});
                    dtw_matr[i+1][j+1] = DtwMartixElement{0, -1, -1};
                }
            }
        }
        return alligns;
    }


}


namespace dtw {
//    double dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws = -1);

    template<typename T>
    DtwAnswer sparse_dtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            std::function<double(const T&, const T&)> dist,
            double Res
    ) {

//        utils::print("In");
        DtwMatrix dtw_matr(ts1.size() + 1);
        for (int i = 0; i <= ts1.size(); i++) {
            dtw_matr[i] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
        }

        dtw_matr[0][0] = DtwMartixElement{0, 0, 0};
        // std::clock_t c_start = std::clock();

        // utils::print("Before norm");
        max_min ts1_m = detail::getMaxMin(ts1);
        max_min ts2_m = detail::getMaxMin(ts2);
        std::vector<T> ts1_w = detail::normFeatureVectorSparce(ts1, ts1_m.max_el, ts1_m.min_el);
        std::vector<T> ts2_w = detail::normFeatureVectorSparce(ts2, ts2_m.max_el, ts2_m.min_el);


      //  PointsVector similarPoints;
        //Bins bins_bounds;
        //double low_bound = 0;
        //double up_bound = Res;
        //while ((low_bound >= 0) and (low_bound <= 1 - Res/2)) {
          //  bins_bounds.push_back(DtwBin{low_bound, up_bound});
          //low_bound = low_bound + Res/2;
        //up_bound = low_bound + Res;

        //}
        //detail::quantFeatureVector<std::vector<T>>(ts1, ts2, bins_bounds, dtw_matr);

        //utils::print("Before q");
        //  similarPoints = detail::quantFeatureVector<std::vector<T>>(ts1_w, ts2_w, bins_bounds);
        //double step = (std::max(ts1_m.max_el, ts2_m.max_el) - std::min(ts1_m.min_el, ts2_m.min_el))*Res;

        double step = Res/2;
       // double step = (std::max(ts1_m.max_el, ts2_m.max_el)-std::min(ts1_m.min_el, ts2_m.min_el))*Res/2;

        //int n=0;
       // int n1=0;
        //printf("%.9f \n",ts1_m.max_el);
        //printf("%.9f \n",ts2_m.max_el);
        //printf("%.9f \n",std::max(ts1_m.max_el, ts2_m.max_el));
        //printf("%.9f \n",std::min(ts1_m.min_el, ts2_m.min_el));
        for (int i = 0; i < ts1.size(); i++) {
            for (int j = 0; j < ts2.size(); j++) {

                if (detail::checkAllign(ts1_w[i], ts2_w[j], step, 2)) {
                    dtw_matr[i+1][j+1] = DtwMartixElement{0, -1, -1};
                   // n+=1;
                    //printf("%.9f %.9f",dtw_matr[i][j].val);
                }
               // n1+=1;
            }
        }
       // printf("%d\n",n);
      //  printf("%d\n",n1);
        dtw_matr[1][1] = DtwMartixElement{0, 0, 0};
        //0, 1: dtw: 10843.77, time: 6.48
        //norm 0, 1: dtw: 12159.72, time: 3.64

        //no norm 0, 1: dtw: 11421.81, time: 3.96
        //norm    0, 1: dtw: 11410.66, time: 4.

        //norm Res 2 0, 1: dtw: 10999.61, time: 6.76
        //norm Res 1 0, 1: dtw: 11426.58, time: 4.05
        //norm Res/2 2 0, 1: dtw: 11410.66, time: 4.06
        //no norm Res 2 0, 1: dtw: 11028.13, time: 7.06

        //0, 1: dtw: 8923.56, time: 2.21
        //0, 1: dtw: 8414.71, time: 4.19

        // printf("%.9f \n",dtw_matr[0][2].val);
        // std::clock_t c_end = std::clock();
        //double time1 = double(c_end - c_start) / (CLOCKS_PER_SEC);
        int i_w;
        int j_w;
        //int n=0;
        for (int i = 1; i <= ts1.size(); i++) {
            for (int j = 1; j <= ts2.size(); j++) {
                if (dtw_matr[i][j].val != INF ){
                    if (dtw_matr[i-1][j].val == INF and dtw_matr[i][j-1].val == INF and dtw_matr[i-1][j-1].val == INF){
                        dtw_matr[i][j] = DtwMartixElement{INF, -1, -1};
                        continue;
                    }
                    else{

                        assignNextDtwMatrixElement(dtw_matr, i, j, dist(ts1[i-1], ts2[j-1]));
                        i_w = std::min<int>(ts1.size(), i + 1);
                        j_w = std::min<int>(ts2.size(), j + 1);




                        if ((dtw_matr[i_w][j].val == INF or i == ts1.size()) and (dtw_matr[i][j_w].val == INF or j == ts2.size()) and dtw_matr[i_w][j_w].val == INF) {

                            if (dtw_matr[i_w][j].val == INF) dtw_matr[i_w][j] = DtwMartixElement{0, -1, -1};
                            if (dtw_matr[i][j_w].val == INF) dtw_matr[i][j_w] = DtwMartixElement{0, -1, -1};
                            if (dtw_matr[i_w][j_w].val == INF) dtw_matr[i_w][j_w] = DtwMartixElement{0, -1, -1};


                        }
                    }
                }
            }

        }

        double dtw_dist = dtw_matr[ts1.size()][ts2.size()].val;

        Path path = untwistDtwMatrixPath(dtw_matr);

        //for (auto point:path){
        //  printf("%d %d\n",point.i,point.j);
        //}
        return DtwAnswer{dtw_dist, path};
    }
}



