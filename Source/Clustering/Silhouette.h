#ifndef PROJECT_TASK1_SILHOUETTE_H
#define PROJECT_TASK1_SILHOUETTE_H

#include "Cluster.h"

vector<double> silhouette(const vector<Cluster *> & clusters,
                          const bool & newSpace=false, vector<Image *> * imgsOldSpace= nullptr);

#endif //PROJECT_TASK1_SILHOUETTE_H
