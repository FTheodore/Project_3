#include "Distance.h"

//Function that calculates the manhattan distance between two images
int manhattanDistance(vector<int> * firstImagePixels, vector<int> * secondImagePixels) {
    int distance = 0;   //Distance to return
    for(int i = 0; i < firstImagePixels->size(); ++i) //For every dimension calculate distance
    {
        int dif = abs(firstImagePixels->at(i) - secondImagePixels->at(i));
        distance += dif;
    }
    return distance; // return distance of images
}

//Function that calculates size W of logical 'grid' from lectures' slides
double calcW(vector<Image *> * imgs, double samplePrcnt, int imgNum) {
    int numIndexes = (samplePrcnt / 100.0) * imgNum;
    vector<int> idx(imgNum); // vector with all image ids.
    iota(idx.begin(), idx.end(), 0); // fill with ids.
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    shuffle(idx.begin(),idx.end(),generator); // shuffle the indexes for randomization
    double dstnc = 0.0;
    unordered_map<string,int> prevDstncs; // keep calculations for time efficiency
    for (int i = 0; i < numIndexes; ++i) {
        /* calculate nearest neighbour from sample */
        int minDistance = numeric_limits<int>::max();
        for (int j = 0; j < numIndexes; ++j) {
            int currentDistance;
            if(i == j)
                continue;
            else if(j < i) { // distance already calculated
                string str = to_string(j)+","+to_string(i);
                currentDistance = prevDstncs.at(str);
            }
            else {
                currentDistance = manhattanDistance(imgs->at(idx.at(i))->getPixels(),
                                                        imgs->at(idx.at(j))->getPixels());
                prevDstncs.insert(make_pair(to_string(i)+","+to_string(j),currentDistance));
            }

            if(currentDistance < minDistance) { // found new nearest image
                minDistance = currentDistance;
            }
        }
        dstnc += minDistance;
    }

    if(dstnc == 0 || numIndexes <= 1) {
        throw runtime_error("W is 0, check sample percentage parameter in \"parameters.conf\" file");
    }

    return dstnc / numIndexes;
}