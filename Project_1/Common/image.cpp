#include "image.h"

Image::Image(int id) {
    this->id = id;
    this->marked = false;
    this->assignedToCluster = false;
    this->pixels = new vector<unsigned char>;
}

int Image::getId() {
    return this->id;
}

Image::~Image() {
    delete this->pixels;
}

void Image::setPixel(unsigned char pxl) {
    this->pixels->push_back(pxl);
}

vector<unsigned char> *Image::getPixels() const {
    return this->pixels;
}

void Image::markImage() {
    this->marked = true;
}

void Image::unmarkImage() {
    this->marked = false;
}


bool Image::isMarked() {
    return this->marked;
}

bool Image::isAssignedToClst() const{
    return this->assignedToCluster;
}

void Image::assignImageToClst(){
    this->assignedToCluster = true;
}

void Image::unassignImageFromClst() {
    this->assignedToCluster = false;
}