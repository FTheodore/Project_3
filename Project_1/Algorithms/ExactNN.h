#ifndef PROJECT_TASK1_EXACTNN_H
#define PROJECT_TASK1_EXACTNN_H

#include <unordered_map>
#include <tuple>
#include <queue>
#include <limits>
#include <chrono>


#include "../Common/image.h"
#include "../Common/Distance.h"
#include "../Common/PriorityQueue.h"

using namespace std;
using namespace std::chrono;


tuple<vector<tuple<double,Image*>>, microseconds> exactNN(Image* queryImage,
                                                       vector<Image *> *datasetImages,
                                                       int numNeighbors, bool=false,
                                                          int=0, int=0, int=0, int=0);

#endif //PROJECT_TASK1_EXACTNN_H
