#include "dataset.h"

Dataset::Dataset(string &fileName, int bytesPerPixel) {
    this->data = new vector<Image *>;

    this->readData(fileName, bytesPerPixel);
}

Dataset::~Dataset(){
    for (int i = 0; i < this->imgNum; ++i) {
        delete this->data->at(i);
    }
    delete this->data; /* delete ptr to vector */
}

void Dataset::readData(string &fileName, int bytesPerPixel) {
    ifstream inpFile(fileName, ios_base::binary);
    if(!inpFile.is_open())
        throw runtime_error("File " + fileName + " cannot be opened.");

    inpFile.seekg(sizeof(int)); /* skip magical number */

    char * p = (char *)&this->imgNum;
    for (int i = 0; i < sizeof(int) * 3; ++i) { /* read 3 ints */
        if(i == sizeof(int)) /* done reading bytes of image # */
            p = (char *)&this->rows;
        else if(i == sizeof(int)*2) /* done reading bytes of rows */
            p = (char *)&this->columns;
        if(i == sizeof(int) || i == sizeof(int)*2 || i == 0)
            p += sizeof(int) - 1; /* set pointer at the mst byte of integers */

        inpFile.get(*p--);
    }

    cout << "Info about file \"" << fileName << "\":" << endl;
    cout << "Number of images:" << this->imgNum << endl;
    cout << "Number of rows:" << this->rows << endl;
    cout << "Number of columns:" << this->columns << endl << endl;

    // create image objects by reading the pixels
    for (int i = 0; i < this->imgNum; ++i) {
        Image * newImg = new Image(i);
        if (bytesPerPixel == 1) {
            for (int j = 0; j < this->rows*this->columns; ++j) {
                newImg->setPixel(inpFile.get());
            }
        }
        else if (bytesPerPixel == 2) {
            for (int j = 0; j < this->rows*this->columns; ++j) {
                int firstByte = 0, secondByte = 0;
                inpFile.get((char &)firstByte);
                inpFile.get((char &)secondByte);
                firstByte = firstByte << 8;
                int pxl = firstByte | secondByte;
                newImg->setPixel(pxl);
            }
        }
        this->data->push_back(newImg);
    }

    inpFile.close();
}

vector<Image *> *Dataset::getImages() const {
    return this->data;
}

int Dataset::getImageNum() const {
    return this->imgNum;
}

int Dataset::getDimensions() const {
    return this->rows * this->columns;
}