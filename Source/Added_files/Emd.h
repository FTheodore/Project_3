#ifndef PROJECT_1_EMD_H
#define PROJECT_1_EMD_H

#include <unordered_map>
#include <cmath>

#include "ortools/linear_solver/linear_solver.h"
#include "../Common/image.h"
#include "../Added_files/ImgClusters.h"

namespace operations_research{
    double emd(vector<int> * firstImg, vector<int> * secondImg, int imageRows, int imageCols, int clstRows, int clstCols);
}

double euclideanDist(tuple<int, int> firstCord, tuple<int, int> secondCord);

#endif //PROJECT_1_EMD_H
