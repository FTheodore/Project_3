#ifndef PROJECT_TASK1_UTILSCLUSTER_H
#define PROJECT_TASK1_UTILSCLUSTER_H

#include "../Common/image.h"
#include "../Common/Distance.h"
#include <cmath>
#include <algorithm>

using namespace std;

vector<int> *getMedian(unordered_map<int,Image *> * imgs, int dimension);

int binSearch(const vector<double> &probs, int start, int end, const double &val);

vector<Image *> kMeansPPlus(vector<Image *> *imgs, int numOfCentroids);

int closestClusterIdx(Image * img, vector<vector<int> *> *centroids);

int minCentroidDist(vector<vector<int> *> *centroids);

vector<int> * centroidOnOldSpace(unordered_map<int, Image *>* clustImgs, vector<Image *> *imagesOldSpace);

#endif //PROJECT_TASK1_UTILSCLUSTER_H
