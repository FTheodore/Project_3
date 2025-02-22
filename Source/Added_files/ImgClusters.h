#ifndef PROJECT_1_IMGCLUSTERS_H
#define PROJECT_1_IMGCLUSTERS_H

#include "../Common/image.h"
#include <tuple>
#include <numeric>
#include <cassert>

class ImgClusters {
private:
    int clstRows;
    int clstCols;

    // each cluster is a tuple of coordinates and normalized weight i.e. brightness
    vector<tuple<tuple<int,int>, double>> clusters;

    int findClustBrightness(vector<int> *, int, int, int);
    tuple<int, int> findClustRepresentative(int, int);
    void findClusters(vector<int> *, int, int);

public:
    ImgClusters(vector<int> *, int, int, int, int);
    vector<tuple<tuple<int,int>, double>> * getClusters();
};


#endif //PROJECT_1_IMGCLUSTERS_H
