#ifndef PROJECT_TASK1_RANGESEARCH_H
#define PROJECT_TASK1_RANGESEARCH_H

#include <unordered_map>
#include <queue>
#include <tuple>
#include <limits>

#include "../Common/image.h"
#include "../Common/Distance.h"
#include "../Structures/lsh.h"
#include "../Structures/HyperCube.h"

vector<tuple<int,Image*>> aproxRangeSrch(vector<int> * queryImage,
                                        Lsh* structure,
                                        double radius,
                                        int threshold);

vector<tuple<int, Image*>> aproxRangeSrch(vector<int> * queryImage,
                                          HyperCube* structure,
                                          int checkThrshld,
                                          int maxProbes,
                                          double radius);

#endif //PROJECT_TASK1_RANGESEARCH_H
