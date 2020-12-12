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

void printResults(tuple<vector<tuple<int,Image*>>, microseconds> &apprNearestImages,
                  tuple<vector<tuple<int,Image*>>, microseconds> &exactNearestImages,
                  vector<tuple<int,Image*>> &apprRangeSrchImages,
                  Image * queryImg,
                  bool,
                  ofstream&);

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
