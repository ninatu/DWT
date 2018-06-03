#include <cmath>

#include "common.hpp"
#include "sparce_dtw.hpp"


namespace detail{
    SpeechTs normFeatureVector(const SpeechTs &vec) {


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

    DoubleTs normFeatureVector(const DoubleTs &vec) {
        double min_elem = INF;
        double max_elem = 0;
        DoubleTs ts;

        for (auto elem : vec) {
            if (elem > max_elem) {
                max_elem = elem;
            }
            if (elem < min_elem) {
                min_elem = elem;
            }
            ts.push_back(elem);
        }
        for (auto &elem : ts) {
            //printf("%.7lf\n",elem);
            elem = (elem - min_elem) / (max_elem - min_elem);
            //printf("%.7lf\n",elem);
        }
        return ts;
    }

    bool checkAllign(double &elem1, double &elem2, Bins &bins){

        bool allign_k = false;
        for (auto bin : bins) {
            if ((elem1 <= bin.j and elem1 >= bin.i) and (elem2 <= bin.j and elem2 >= bin.i)) {
                allign_k = true;
                break;
            }
        }
        return allign_k;

    }

    bool checkAllign(SpeechTsElem &elem1, SpeechTsElem &elem2, Bins &bins){

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
    PointsVector quantFeatureVector(T &ts1, T &ts2, Bins &bins) {

        PointsVector alligns;
        utils::print("Inll");
        //double one = ts1[0];
       // printf("%.7lf\n",ts1[0]);
       // printf("%.7lf\n",ts1[ts1.size()]);
        utils::print("Inll");


        for (int i = 0; i < ts1.size(); i++) {
            for (int j = 0; j < ts2.size(); j++) {

                if (detail::checkAllign(ts1[i], ts2[j], bins)){
                    alligns.push_back(DtwPathElement{i,j});
                }
            }
        }
        return alligns;
    }

}


namespace dtw {
//    double dtw(const std::vector<double> &ts1, const std::vector<double> &ts2, int ws = -1);

    template<typename T>
    DtwAnswer sparce_dtw(
            const std::vector<T> &ts1,
            const std::vector<T> &ts2,
            std::function<double(const T&, const T&)> dist,
            double Res = 0.5
    ) {
//        if (window < 0) {
  //          window = int(ts2.size());
    //    }
        utils::print("In");
        DtwMatrix dtw_matr(ts1.size() + 1);
        for (int i = 0; i <= ts1.size(); i++) {
            dtw_matr[i] = std::vector<DtwMartixElement>(ts2.size() + 1, DtwMartixElement{INF, 0, 0});
        }

        dtw_matr[0][0] = DtwMartixElement{0, 0, 0};

        utils::print("Before norm");
        std::vector<T> ts1_w = detail::normFeatureVector(ts1);
        std::vector<T> ts2_w = detail::normFeatureVector(ts2);
        utils::print("After norm");

       // printf("%.7lf\n",ts1[7]);
     //   printf("%.7lf\n",ts1_w[7]);

        //std::vector<std::vector<DtwPathElement>> refs;

//i_bin = 0;
// i_ref = 0;
        Bins bins_bounds;
        double low_bound = 0;
        double up_bound = Res;
        printf("%.9f\n",Res);

        PointsVector similarPoints;

        while ((low_bound >= 0) and (low_bound <= 1 - Res/2)) {
            bins_bounds.push_back(DtwBin{low_bound, up_bound});
         //   printf("%.7lf\n",low_bound);
           // printf("%.7lf\n",up_bound);
            //printf("%.7lf\n",Res);
            low_bound = low_bound + Res/2;
            up_bound = low_bound + Res;

        }
        utils::print("Before q");
        similarPoints = detail::quantFeatureVector<std::vector<T>>(ts1_w, ts2_w, bins_bounds);
        utils::print("After q");


        for (auto point : similarPoints){
            dtw_matr[point.i+1][point.j+1] = DtwMartixElement{dist(ts1_w[point.i], ts2_w[point.j]), 0, 0};
            }
        double cost;
        int i_w;
        int j_w;
        int n=0;
        for (int i = 1; i <= ts1.size(); i++) {
            for (int j = 1; j <= ts2.size(); j++) {
                if (dtw_matr[i][j].val != INF){
                    //if (n < 100){
                      //  printf("%.7f %d %d\n",dtw_matr[i][j].val, i, j);
                       // n+= 1;
                    //}
                   // printf("%.7f %d %d\n",dtw_matr[i][j].val, i, j);
                    //n+= 1;
                    cost = dist(ts1[i], ts2[j]);
                  //  printf("%.7f\n",cost);
                    assignNextDtwMatrixElement(dtw_matr, i, j, cost);
                    i_w = std::min<int>(ts1.size(), i + 1);
                    j_w = std::min<int>(ts2.size(), j + 1);
                   // utils::print("in");

                    if ((dtw_matr[i_w][j].val == INF or i == ts1.size()) and (dtw_matr[i][j_w].val == INF or j == ts2.size()) and dtw_matr[i_w][j_w].val == INF) {
                        assignNextDtwMatrixElement(dtw_matr, i_w, j, 0);
                        assignNextDtwMatrixElement(dtw_matr, i, j_w, dist(ts1[i], ts2[j_w]));
                        assignNextDtwMatrixElement(dtw_matr, i_w, j_w, 0);
                    }
                }
            }

        }
        utils::print("After all");
        double dtw_dist = dtw_matr[ts1.size()][ts2.size()].val;
        Path path = untwistDtwMatrixPath(dtw_matr);
       // for (auto point:path){
         //   printf("%d %d\n",point.i,point.j);
        //}
        return DtwAnswer{dtw_dist, path};
    }
}