#ifndef PROJECT_TASK1_ASSIGNMENT_H
#define PROJECT_TASK1_ASSIGNMENT_H

#include "../Common/Utils.h"
#include "UtilsCluster.h"
#include "Cluster.h"
#include "../Structures/lsh.h"
#include "../Algorithms/RangeSearch.h"

void lloydAssign(const vector<Cluster *> & clusters, vector<Image *> *imgs); // first assignment
void lloydAssign(const vector<Cluster *> & clusters, int & assignmentsNum);

void reverseAssign(const vector<Cluster *> & clusters,
                   vector<Image *> *allImgs,
                   Lsh* lsh,
                   int & totalChanges,
                   HyperCube* hpcb,
                   int checkThrshld,
                   int maxProbes,
                   int srchThresh,
                   int clustThresh);

#endif //PROJECT_TASK1_ASSIGNMENT_H
