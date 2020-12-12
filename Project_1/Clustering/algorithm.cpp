#include "algorithm.h"

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
                              int clustIters) {
    int imgNum = imgs.size();
    cout << "Initial Centroids calculation... " << endl;
    vector<Image *> centroids = kMeansPPlus(&imgs, numClusters); // choose initial centroids
    cout << "Done" << endl;
    vector<Cluster *> * clusters = makeClusters(&centroids, numClusters ); // create clusters
    if(assignMethod == "Classic")
        lloydAssign(*clusters, &imgs); // initial assignment of images to clusters
    int iterations = 0;
    while (iterations < imgNum || iterations < clustIters) {
        int assignmentsPerformed = 0;
        if(assignMethod != "Classic") {
            reverseAssign(*clusters, allImgs, lsh, assignmentsPerformed, hpbc, checkThrshld,
                          maxProbes, srchThresh, clustThresh);
        }

        for (Cluster* const &clst: *clusters) { //Images changed cluster, update centroid
            clst->updateCentroid();
        }

        if(assignMethod == "Classic") {
            lloydAssign(*clusters, assignmentsPerformed);
        }

        if(assignmentsPerformed <= clustThresh){ // convergence witnessed
            break;
        }

        ++iterations;
    }

    return clusters;
}