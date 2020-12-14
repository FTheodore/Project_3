#ifndef PROJECT_TASK1_DATASET_H
#define PROJECT_TASK1_DATASET_H

#include <vector>
#include <fstream>
#include <unordered_map>

#include "image.h"

using namespace std;

class Dataset { //Class that contains all images read from a single file
private:
    // dimensions of images
    int rows;
    int columns;
    int imgNum;
    vector<Image *> * data; // vector that contains the images
    void readData(string &, int);

public:
    Dataset(string &, int);
    virtual ~Dataset();

    vector<Image *> *getImages() const;
    int getImageNum() const;
    int getDimensions() const;
};


#endif //PROJECT_TASK1_DATASET_H