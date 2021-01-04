#ifndef PROJECT_TASK1_ALGORITHM_H
#define PROJECT_TASK1_ALGORITHM_H

#include "Assignment.h"

vector<Cluster *> *clustering(const string & assignMethod,
                              vector<Image *> imgs,
                              vector<Image *> *allImgs,
                              int numClusters,
                              int checkThrshld,
                              int maxProbes,
                              Lsh * lsh,
                              HyperCube * hpbc,
                              int srchThresh,
                              int clustThresh,
                              int clustIters);

#endif //PROJECT_TASK1_ALGORITHM_H
