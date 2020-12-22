#include "Cluster.h"

vector<Cluster *> * makeClusters(vector<Image *> *centroids, int numClusters) {
    vector<Cluster *> * vec = new vector<Cluster *>;
    vec->reserve(numClusters);
    for (int i = 0; i < numClusters; ++i) {
        Cluster * newClust = new Cluster(centroids->at(i));
        vec->push_back(newClust);
    }
    return vec;
}

vector<Cluster *> * makeClustersFromFile(vector<Image *> * images,
                                         const string & clustFile, const int & dimension) {
    vector<Cluster *> * vec = new vector<Cluster *>;

    // open file with clusters' info
    ifstream inpFile(clustFile);
    if(!inpFile.is_open())
        throw runtime_error("File " + clustFile + " cannot be opened.");

    string line;
    while (getline(inpFile,line)) {
        if(line.find("CLUSTER")==string::npos || line.find("size: 0")!=string::npos) {
            continue;
        }
        Cluster * newClust = new Cluster(images, line, dimension);
        vec->push_back(newClust);
    }
    return vec;
}

void gatherCentroids(const vector<Cluster *> & clusters, vector<vector<int> *> *vec) {
    for (int i = 0; i < clusters.size(); ++i)
        vec->push_back(clusters.at(i)->getCentroid());
}

void gatherCentroidsOldSpc(const vector<Cluster *> & clusters, vector<vector<int> *> *vec,
                           vector<Image*> * imagesOldSpc) {
    for (int i = 0; i < clusters.size(); ++i)
        vec->push_back(centroidOnOldSpace(clusters.at(i)->getClusterImgs(), imagesOldSpc));
}

//Get index of current cluster of the image
int getImgCluster(const vector<Cluster *> & clusters, Image * img) {
    int toRet = -1;
    for (int i = 0; i < clusters.size(); ++i) {
        if(clusters.at(i)->getClusterImgs()->end() != clusters.at(i)->getClusterImgs()->find(img->getId())) {
            toRet = i;
            break;
        }
    }
    return toRet;
}

bool imgIsCentroid(const vector<Cluster *> & clusters, Image * img) {
    bool toRet = false;
    for (int i = 0; i < clusters.size(); ++i) {
        if(clusters.at(i)->centroidIsImg() && clusters.at(i)->getCentrId() == img->getId()) {
            toRet = true;
            break;
        }
    }
    return toRet;
}

Cluster::Cluster(Image * centroidImg) {
    this->centroid = centroidImg->getPixels();
    this->centrIsInDataset = true;
    this->firstCentroidPtr = centroidImg; // keep the Image for the centroid update
    this->imgs_in_cluster = new unordered_map<int,Image *>;
}

Cluster::Cluster(vector<Image *> * images, const string & infoLine, int dimension) {
    this->centrIsInDataset = false;
    this->firstCentroidPtr = nullptr;
    this->imgs_in_cluster = new unordered_map<int,Image *>;

    // process image id's from clusters' file and add them to current cluster
    std::istringstream iss(infoLine);
    string token;
    bool sizeNext = false;
    while(iss >> token) {
        if (token.find("CLUSTER")!=string::npos ||
            sizeNext) {
            if (sizeNext) {
                sizeNext = false;
            }
            continue;
        } else if(token.find("size")!=string::npos) {
            sizeNext = true;
            continue;
        }

        string idStr = token.substr(0, token.length()-1);
        int imgId = stoi(idStr);

        this->addImg(images->at(imgId));

    }

    this->centroid = getMedian(this->imgs_in_cluster, dimension);
}


Cluster::~Cluster() {
    delete this->imgs_in_cluster;
    if(!this->centrIsInDataset)
        delete this->centroid;
}


void Cluster::updateCentroid() {
    int dimension = this->centroid->size();
    if(this->centrIsInDataset) { // make sure not to lose the image upon updating centroid
        this->centrIsInDataset = false;
        this->imgs_in_cluster->insert(make_pair(this->firstCentroidPtr->getId(),this->firstCentroidPtr));
        this->firstCentroidPtr = nullptr;
    } else {
        delete this->centroid;
    }
    this->centroid = getMedian(this->imgs_in_cluster,dimension);
}

unordered_map<int,Image *> * Cluster::getClusterImgs() {
    return this->imgs_in_cluster;
}

bool Cluster::centroidIsImg() const {
    return this->centrIsInDataset;
}

int Cluster::getCentrId() const {
    return this->firstCentroidPtr->getId();
}

vector<int> * Cluster::getCentroid() {
    return this->centroid;
}

unordered_map<int,Image *>::iterator Cluster::removeImg(unordered_map<int,Image *>::iterator it) {
    return this->imgs_in_cluster->erase(it);
}

void Cluster::removeImg(const int & key) {
    this->imgs_in_cluster->erase(key);
}

void Cluster::addImg(Image * newImg) {
    this->imgs_in_cluster->insert(make_pair(newImg->getId(),newImg));
}

void printClstrRslts(ofstream & outputFile, vector<Cluster *> * clusters,
                     string *dur, vector<double> *silhouetteRes,
                     const int & objective, bool classification, bool newSpace) {
    if(!classification) {
        //Print algorithm
        if (!newSpace)
            outputFile << "ORIGINAL SPACE" << endl;
        else
            outputFile << "NEW SPACE" << endl;

        //Print Clusters size and centroid pixels
        for (int i = 0; i < clusters->size(); ++i) {
            string clust = "CLUSTER-" + to_string(i + 1) + " { size: " +
                           to_string(clusters->at(i)->getClusterImgs()->size()) + ", centroid: <";
            for (int px = 0; px < clusters->at(i)->getCentroid()->size(); ++px) {
                clust += to_string(clusters->at(i)->getCentroid()->at(px)) + ", ";
            }
            clust = clust.substr(0, clust.size() - 2);
            clust += "> }";
            outputFile << clust << endl;
        }

        outputFile << "clustering_time: " << *dur << endl;
    }
    else {
        outputFile << "CLASSES AS CLUSTERS" << endl;
    }

    string silhouetteRet = "Silhouette: [";
    for(int i = 0; i < silhouetteRes->size(); ++i)
    {
        silhouetteRet += to_string(silhouetteRes->at(i)) + ", ";
    }
    silhouetteRet = silhouetteRet.substr(0, silhouetteRet.size()-2);
    silhouetteRet += "(s_total)]";
    outputFile << silhouetteRet << endl;
    outputFile << "Value of Objective Function: " << objective << endl;
    outputFile << endl;

}