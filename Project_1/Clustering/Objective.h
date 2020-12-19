#ifndef PROJECT_1_OBJECTIVE_H
#define PROJECT_1_OBJECTIVE_H

#include "../Common/image.h"
#include "../Common/Distance.h"
#include "Cluster.h"

int calcObjective(const vector<Cluster *> & clusters, bool newSpace = false, vector<Image *> * oldImgs = nullptr);

#endif //PROJECT_1_OBJECTIVE_H
