#include "Labels.h"

Labels::Labels(string & inputLblsFile, string & queryLblsFile) {
    this->readLabels(inputLblsFile, true);
    this->readLabels(queryLblsFile, false);
}

void Labels::readLabels(string & fileName, bool isInput) {
    ifstream inpFile(fileName, ios_base::binary);
    if(!inpFile.is_open())
        throw runtime_error("File " + fileName + " cannot be opened.");

    inpFile.seekg(sizeof(int)); /* skip magical number */

    // get amount of labels in file
    int lblsNum;
    for (int i = 0; i < sizeof(int); ++i) {
        char * p = (char *)&lblsNum;
        p += sizeof(int) - i - 1;
        inpFile.get(*p);
    }

    cout << "File \"" << fileName << "\" has " << lblsNum << " labels" << endl << endl;

    // get label for every image and save pair to corresponding map
    for (int i = 0; i < lblsNum; ++i) {
        unsigned char lbl = inpFile.get();
        if(isInput)
            this->inputLabels.insert(make_pair(i,lbl));
        else
            this->queryLabels.insert(make_pair(i,lbl));
    }

    inpFile.close();
}

double Labels::correctPredictions(Image * queryImg, vector<tuple<double, Image *>> * nearestImages) {
    // label of query image
    unsigned char correctLabel = this->queryLabels.at(queryImg->getId());
    // amount of correct predictions
    int correctPreds = 0;
    // find nearest images with same label
    for (const tuple<double,Image*> &tpl: *nearestImages) {
        int nearestImgId = get<1>(tpl)->getId();
        if(this->inputLabels.at(nearestImgId) == correctLabel)
            ++correctPreds;
    }
    return (double)correctPreds / nearestImages->size();
}