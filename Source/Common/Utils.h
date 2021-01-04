#ifndef PROJECT_TASK1_UTILS_H
#define PROJECT_TASK1_UTILS_H

#include <tuple>
#include <chrono>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "image.h"
#include "Distance.h"

using namespace std;
using namespace std::chrono;

void printResults(tuple<vector<tuple<double,Image*>>, microseconds> &apprNearestImages,
                  tuple<vector<tuple<double,Image*>>, microseconds> &exactNearestImages,
                  vector<tuple<double,Image*>> &apprRangeSrchImages,
                  Image * queryImg,
                  bool,
                  ofstream&);

void printComparison(tuple<vector<tuple<double,Image*>>, microseconds> &exactOldSpcNearestImage,
                     tuple<vector<tuple<double,Image*>>, microseconds> &exactNewSpcNearestImage,
                     tuple<vector<tuple<double,Image*>>, microseconds> &lshOldSpcNearestImage,
                     int exactNewOrigDist,
                     Image * queryImg,
                     bool isLsh,
                     ofstream& outputFile);

void approxFactor(vector<double> apprFactorTermsLSH,
                  vector<double> apprFactorTermsReduced,
                  ofstream& outputFile);

void unmarkImgs(vector<Image*> * imgs, int imgNum);
void unassignImgs(vector<Image*> * imgs, int imgNum);

void getNearbyVertices(vector<string> &vec,
                       string currentVertex,
                       int i,
                       int changesLeft);

void getVerticesToCheck(vector<string> &vec,
                        string &currentVertex,
                        int maxDistance);

void readParams(int & w_smpl_prcnt, int & w_factor,
                bool readApproxThresh=false, int * approx_threshold= nullptr,
                bool readClstThresh=false, int * clust_threshold= nullptr,
                bool readClstThreshIters=false, int * clust_iters= nullptr);

#endif //PROJECT_TASK1_UTILS_H
