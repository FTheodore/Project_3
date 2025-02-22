#ifndef PROJECT_TASK1_CLUSTER_H
#define PROJECT_TASK1_CLUSTER_H

#include <fstream>
#include <sstream>

#include "UtilsCluster.h"

class Cluster {
private:
    vector<int> *centroid; //Pixels of centroid
    Image * firstCentroidPtr; //keep pointer to first image centroid
    bool centrIsInDataset; // true if centroid is an image
    unordered_map<int,Image *> * imgs_in_cluster; // images inside the cluster
public:
    Cluster(Image *);
    Cluster(vector<Image *> *, const string &, int);
    ~Cluster();
    void updateCentroid();
    unordered_map<int,Image *> * getClusterImgs();
    vector<int> * getCentroid();
    unordered_map<int,Image *>::iterator removeImg(unordered_map<int,Image *>::iterator);
    void removeImg(const int & key);
    void addImg(Image *);
    bool centroidIsImg() const;
    int getCentrId() const;
};

vector<Cluster *> * makeClusters(vector<Image *> *centroids, int numClusters);
vector<Cluster *> * makeClustersFromFile(vector<Image *> * images,
                                         const string & clustFile, const int & dimension);
void gatherCentroids(const vector<Cluster *> & clusters, vector<vector<int> *> *vec);
int getImgCluster(const vector<Cluster *> & clusters, Image * img);
bool imgIsCentroid(const vector<Cluster *> & clusters, Image * img);

void printClstrRslts(ofstream & outputFile, vector<Cluster *> * clusters,
                     string *dur, vector<double> *silhouetteRes,
                     const int & objective, bool classification, bool newSpace = false);

void gatherCentroidsOldSpc(const vector<Cluster *> & clusters, vector<vector<int> *> *vec,
                           vector<Image*> * imagesOldSpc);

#endif //PROJECT_TASK1_CLUSTER_H
