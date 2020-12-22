#include "Objective.h"

// find cluster distance that is closest to the corresponding image
int closestClusterDist(Image * img, vector<vector<int> *> * centroids) {
    int minDistance = numeric_limits<int>::max();
    for (vector<int> * centroid: *centroids) {
        int newDist = manhattanDistance(img->getPixels(), centroid);
        if(minDistance > newDist) {
            minDistance = newDist;
        }
    }
    return minDistance;
}

// calculates objective function value
int calcObjective(const vector<Cluster *> & clusters, bool newSpace, vector<Image *> * oldImgs) {
    vector<vector<int> *> centroids;
    if(!newSpace)
        gatherCentroids(clusters, &centroids);
    else
        gatherCentroidsOldSpc(clusters, &centroids, oldImgs);

    int objective = 0;
    for (Cluster * clust: clusters) {
        unordered_map<int, Image *> * clustImgs = clust->getClusterImgs();
        for (pair<const int, Image *> & pair: *clustImgs) {
            objective += closestClusterDist(pair.second, &centroids);
        }
    }

    if(newSpace) {
        for(vector<int> * centroid: centroids) delete centroid;
    }

    return objective;
}