#include "UtilsCluster.h"

vector<int> *getMedian(unordered_map<int,Image *> * imgs, int dimension) {
    // a vector containing median for each dimension
    vector<int> * toRet = new vector<int>;

    for (int i = 0; i < dimension; ++i) {
        vector<int> dim_i_pixels;
        dim_i_pixels.reserve(imgs->size());
        for (const pair<const int,Image *> & pair: *imgs) { // gather all pixels for each dimension
            dim_i_pixels.push_back(pair.second->getPixels()->at(i));
        }
        // sort in order to find the median
        sort(dim_i_pixels.begin(), dim_i_pixels.end());
        int medianIdx = floor((dim_i_pixels.size()-1) / 2);
        toRet->push_back(dim_i_pixels.at(medianIdx)); // keep the median of current dimension
    }

    return toRet;
}

int closestClusterIdx(Image * img, vector<vector<int> *> *centroids) {
    int minDistance = numeric_limits<int>::max();
    int clustIdx;
    for (int i = 0; i < centroids->size(); ++i) {
        int newDist = manhattanDistance(img->getPixels(), centroids->at(i));
        if(minDistance > newDist) {
            minDistance = newDist;
            clustIdx = i;
        }
    }
    return clustIdx;
}

int closestImgToCentroid(unordered_map<int, Image *>* clustImgs, vector<int> *centroid) {
    int minDistance = numeric_limits<int>::max();
    int imgId;
    for (pair<const int, Image *> & pair: *clustImgs) {
        int newDist = manhattanDistance(pair.second->getPixels(), centroid);
        if(minDistance > newDist) {
            minDistance = newDist;
            imgId = pair.second->getId();
        }
    }
    return imgId;
}

int minCentroidDist(vector<vector<int> *> *centroids) {
    int minDistance = numeric_limits<int>::max();

    for (int i = 0; i < centroids->size(); ++i) {
        for (int z = i+1; z < centroids->size(); ++z) {
            int newDist = manhattanDistance(centroids->at(z), centroids->at(i));
            if(minDistance > newDist) {
                minDistance = newDist;
            }
        }
    }

    return minDistance/2;
}

/* Functions used for kmeans++ initialization */

double minDistance(Image * img, vector<Image *> *centroids) {
    double minDistance = numeric_limits<double>::infinity();
    for (Image* const &centroid : *centroids) {
        int newDist = manhattanDistance(img->getPixels(), centroid->getPixels());
        if(minDistance > newDist)
            minDistance = newDist;
    }
    return minDistance;
}

vector<double> calcMinDistances(vector<Image *> *imgs, vector<Image *> *centroids) {
    vector<double> toRet;
    toRet.push_back(0.0); // P(0) = 0
    for(Image* const &img: *imgs) {
        toRet.push_back(minDistance(img,centroids));
    }
    return toRet;
}

void normalizeDistances(vector<double> * distances) {
    double maxDist = *max_element(distances->begin(), distances->end());
    for (double &dist: *distances) {
        dist /= maxDist;
    }
}

vector<double> getPropabilities(vector<Image *> *centroids, vector<Image *> *imgs) {
    vector<double> toRet = calcMinDistances(imgs, centroids);
    normalizeDistances(&toRet); //normalize all D(i)'s
    for (int i = 1; i < toRet.size(); ++i) {
        toRet.at(i) = pow(toRet.at(i),2) + toRet.at(i-1);   //make use of previous calculations
    }
    return toRet;
}

int binSearch(const vector<double> &probs, int start, int end, const double &val) {
    if(start == end) { // reached last element
        if(val <= probs.at(start)) { // the last element is P(r)
            return start;
        }
        else if(val > probs.at(start)) { // the last element is P(r-1)
            return start + 1;
        }
    }

    if(start > end) { // P(r) is on the right
        return start;
    }

    int midl = floor((start + end) / 2);
    if(val > probs.at(midl)) // P(r) is in the right array
        return binSearch(probs, midl+1, end, val);
    else if(val < probs.at(midl)) // P(r) is in the left array
        return binSearch(probs, start, midl-1, val);
    else // current element is P(r)
        return midl;
}

vector<Image *> kMeansPPlus(vector<Image *> *imgs, int numOfCentroids) {
    unsigned seed = system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0, imgs->size()-1);

    int index = distribution(generator); //Pick a random image for the first centroid

    vector<Image *> centroids;
    centroids.push_back(imgs->at(index));
    imgs->erase(imgs->begin() + index); //Keep non centroid images
    double minDoub = numeric_limits<double>::min();
    for(int i = 1; i < numOfCentroids; ++i) { //Repeat until all initial centroids are found
        vector<double> probs = getPropabilities(&centroids, imgs); //Get vector of P(r)’s

        uniform_real_distribution<double> dist(0 + minDoub, probs.at(probs.size() - 1) + minDoub);
        double rand_num = dist(generator);

        //get r for which random number is in range of (P(r-1),P(r)]
        index = binSearch(probs,0, probs.size()-1, rand_num);

        centroids.push_back(imgs->at(index));
        imgs->erase(imgs->begin() + index);
    }

    return centroids;
}