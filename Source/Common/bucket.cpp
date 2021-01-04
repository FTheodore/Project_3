#include "bucket.h"

Bucket::Bucket() {
    this->images = new vector<Image *>;
    this->g_hash_results = new vector<unsigned int>;
}

Bucket::~Bucket() {
    delete this->images;
    delete this->g_hash_results;
}

//insert a new image to the bucket
void Bucket::insertImage(Image * img) {
    this->images->push_back(img);
}

//get the images of the bucket
vector<Image *> *Bucket::getImages() {
    return this->images;
}

//insert g hash function result before modulo of the image that is currently being inserted to the bucket
void Bucket::insertHashRes(unsigned int res) {
    this->g_hash_results->push_back(res);
}

//get bucket images' hashes
vector<unsigned int> *Bucket::getHashReslts() {
    return this->g_hash_results;
}