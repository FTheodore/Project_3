#ifndef PROJECT_1_LABELS_H
#define PROJECT_1_LABELS_H

#include "../Common/image.h"
#include <fstream>
#include <unordered_map>

class Labels {
private:
    // maps of image id to label
    unordered_map<int, unsigned char> queryLabels;
    unordered_map<int, unsigned char> inputLabels;

    void readLabels(string &, bool);

public:
    Labels(string &, string &);

    double correctPredictions(Image *, vector<tuple<double,Image*>> *);
};


#endif //PROJECT_1_LABELS_H
