#ifndef PROJECT_TASK1_UTILSCLUSTER_H
#define PROJECT_TASK1_UTILSCLUSTER_H

#include "../Common/image.h"
#include "../Common/Distance.h"
#include <cmath>
#include <algorithm>

using namespace std;

vector<unsigned char> *getMedian(unordered_map<int,Image *> * imgs, int dimension);

int binSearch(const vector<double> &probs, int start, int end, const double &val);

vector<Image *> kMeansPPlus(vector<Image *> *imgs, int numOfCentroids);

int closestClusterIdx(Image * img, vector<vector<unsigned char> *> *centroids);

int minCentroidDist(vector<vector<unsigned char> *> *centroids);

#endif //PROJECT_TASK1_UTILSCLUSTER_H
