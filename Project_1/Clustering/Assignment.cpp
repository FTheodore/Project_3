#include "Assignment.h"


void lloydAssign(const vector<Cluster *> & clusters, vector<Image *> *imgs) { // first assignment, clusters are empty
    vector<vector<unsigned char> *> centroids;
    gatherCentroids(clusters,&centroids);

    //Iterate over all images to be inserted into clusters
    for (int i = 0; i < imgs->size(); ++i) {
        int closestClust = closestClusterIdx(imgs->at(i),&centroids); //find closest cluster
        clusters.at(closestClust)->addImg(imgs->at(i)); //insert image to closest cluster
    }
}

void lloydAssign(const vector<Cluster *> & clusters, int & assignmentsNum) {
    vector<vector<unsigned char> *> centroids;
    gatherCentroids(clusters,&centroids);

    unordered_map<int,Image *> * clustImgs;
    //Iterate over all clusters
    for (int i = 0; i < clusters.size(); ++i) {
        clustImgs = clusters.at(i)->getClusterImgs();
        unordered_map<int,Image *>::iterator it = clustImgs->begin();
        //Iterate over all cluster images
        while(it != clustImgs->end()) {
            int closestClust = closestClusterIdx(it->second,&centroids);
            if(closestClust != i) { // reassign image to new cluster
                clusters.at(closestClust)->addImg(it->second);
                it = clusters.at(i)->removeImg(it);
                ++assignmentsNum;
            }
            else
                ++it;
        }
    }
}

using searchResults = vector<tuple<int,Image*>>;

// return the iterator of the next image to be assigned to a cluster out of all range searches' results
int nextIt(vector<searchResults::iterator> *iterators, vector<searchResults> * results) {
    int minDistance = numeric_limits<int>::max();
    int nextIt = -1; // -1 will be returned in case all range searches' results have been processed
    for(int i = 0; i < iterators->size(); ++i) {
        if(iterators->at(i) == results->at(i).end()) // processed all results of a single range search
            continue;
        // found an image with a smaller distance to one of the centroids
        if(get<0>(*iterators->at(i)) < minDistance) {
            minDistance =  get<0>(*iterators->at(i));
            nextIt = i;
        }
    }
    return nextIt;
}

int processResults(vector<searchResults> * results, const vector<Cluster *> & clusters) {
    vector<searchResults::iterator> iterators;
    iterators.reserve(results->size()); // iterators will point to the first <distance,Image> pair for each range search
    for (int i = 0; i < results->size(); ++i) { // initialize iterators to every range search results' vector beginning
        iterators.push_back(results->at(i).begin());
    }
    int newAssignments = 0; // keep count of the # of images that were assigned to new clusters
    int itIdx = nextIt(&iterators, results);
    while(itIdx != -1) { // while all results have not been assessed
        Image * imgPtr = get<1>(*iterators.at(itIdx)); // pointer to next image that will be assigned
        // image can't switch clusters since it is a centroid or the image has already been assigned to a closer cluster
        if(imgIsCentroid(clusters, imgPtr) || imgPtr->isAssignedToClst()) {
            ++iterators.at(itIdx);
            itIdx = nextIt(&iterators, results);
            continue;
        }
        int closestClust = itIdx;
        int currClust = getImgCluster(clusters, imgPtr);
        if(closestClust != currClust) { // image's current cluster is not the closest one
            clusters.at(closestClust)->addImg(imgPtr);
            if(currClust != -1) { //image is not assigned to a cluster for the first time, so remove it from the old one
                clusters.at(currClust)->removeImg(imgPtr->getId());
            }
            ++newAssignments;
        }
        // mark the image as already assigned to a cluster that is closer from all next range search results
        imgPtr->assignImageToClst();
        ++iterators.at(itIdx);
        itIdx = nextIt(&iterators, results);
    }
    return newAssignments;
}

void reverseAssign(const vector<Cluster *> & clusters,
                   vector<Image *> *allImgs,
                   Lsh* lsh,
                   int & totalChanges,
                   HyperCube* hpcb,
                   int checkThrshld,
                   int maxProbes,
                   int srchThresh,
                   int clustThresh) {
    vector<vector<unsigned char> *> centroids;
    gatherCentroids(clusters,&centroids);
    int radius = minCentroidDist(&centroids);
    while(true) {
        vector<searchResults> results;
        for (int i = 0; i < centroids.size(); ++i) { // perform range search for every centroid
            if(hpcb == nullptr)
                results.push_back(aproxRangeSrch(centroids.at(i),lsh,radius,srchThresh));
            else
                results.push_back(aproxRangeSrch(centroids.at(i),hpcb,checkThrshld,maxProbes,radius));

            unmarkImgs(allImgs, allImgs->size());
        }
        int changes = processResults(&results, clusters); // process the results and possibly reassign images
        // keep track of the total # of images that have been reassigned in current assignment step
        totalChanges += changes;
        if(changes <= clustThresh)
            break;
        radius*=2;
    }

    // lloyd's assignment for all images that weren't located by range search
    for (int i = 0; i < allImgs->size(); ++i) {
        Image *imgPtr = allImgs->at(i);
        if (imgIsCentroid(clusters, imgPtr) || imgPtr->isAssignedToClst())
            continue;

        int closestClust = closestClusterIdx(imgPtr, &centroids);
        int currClust = getImgCluster(clusters, imgPtr);
        if (closestClust != currClust) {
            clusters.at(closestClust)->addImg(imgPtr);
            ++totalChanges;
            if (currClust != -1) {
                clusters.at(currClust)->removeImg(imgPtr->getId());
            }
        }
    }

    unassignImgs(allImgs, allImgs->size());
}

