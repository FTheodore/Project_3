#include "Objective.h"

// find cluster distance that is closest to the corresponding image
int closestClusterDist(Image * img, const vector<Cluster *> & clusters, bool newSpace, vector<Image *> * oldImages) {
    int minDistance = numeric_limits<int>::max();
    for (int i = 0; i < clusters.size(); ++i) {
        int newDist;
        if(!newSpace)
            newDist = manhattanDistance(img->getPixels(), clusters.at(i)->getCentroid());
        else {
            // centroid on old space will be closest img to centroid on new space
            int closestImgId = closestImgToCentroid(clusters.at(i)->getClusterImgs(), clusters.at(i)->getCentroid());
            newDist = manhattanDistance(oldImages->at(img->getId())->getPixels(),
                                        oldImages->at(closestImgId)->getPixels());
        }
        if(minDistance > newDist) {
            minDistance = newDist;
        }
    }
    return minDistance;
}

// calculates objective function value
int calcObjective(const vector<Cluster *> & clusters, bool newSpace, vector<Image *> * oldImgs) {
    int objective = 0;
    for (int i = 0; i < clusters.size(); ++i) {
        unordered_map<int, Image *> * clustImgs = clusters.at(i)->getClusterImgs();
        for (pair<const int, Image *> & pair: *clustImgs) {
            objective += closestClusterDist(pair.second, clusters, newSpace, oldImgs);
        }
    }
    return objective;
}