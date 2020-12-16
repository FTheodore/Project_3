#ifndef PROJECT_1_EMD_H
#define PROJECT_1_EMD_H

#include <unordered_map>
#include <cmath>

#include "../Common/image.h"
#include "../Added_files/ImgClusters.h"

double emd(vector<int> * firstImg, vector<int> * secondImg, int imageRows, int imageCols, int clstRows, int clstCols);

double euclideanDist(tuple<int, int> firstCord, tuple<int, int> secondCord);

#endif //PROJECT_1_EMD_H
