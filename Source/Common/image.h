#ifndef PROJECT_TASK1_IMAGE_H
#define PROJECT_TASK1_IMAGE_H

#include <iostream>
#include <vector>


using namespace std;

class Image {   //contains a single image
private:
    int id; //image unique id based on file position
    bool marked;    //Lsh flag that is true when image was found in previous lsh tables
    bool assignedToCluster;
    vector<int> *pixels;

public:
    Image(int);
    ~Image();
    int getId();
    void setPixel(int);

    vector<int> *getPixels() const;
    bool isMarked();
    void markImage();
    void unmarkImage();
    bool isAssignedToClst() const;
    void assignImageToClst();
    void unassignImageFromClst();
};


#endif //PROJECT_TASK1_IMAGE_H