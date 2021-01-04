#ifndef PROJECT_TASK1_BUCKET_H
#define PROJECT_TASK1_BUCKET_H

#include <vector>
#include "image.h"

class Bucket{ // implements the buckets/vertices of the lsh/hypercube structures
private:
    // Vector containing images of this bucket
    vector<Image *> *images;
    // Vector containing the g hash function result before modulo operation was applied
    vector<unsigned int> *g_hash_results;
public:
    Bucket();
    ~Bucket();
    void insertImage(Image *);
    vector<Image *> *getImages();
    void insertHashRes(unsigned int res);
    vector<unsigned int> *getHashReslts();
};

#endif //PROJECT_TASK1_BUCKET_H
