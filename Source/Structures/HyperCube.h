#ifndef PROJECT_TASK1_HYPERCUBE_H
#define PROJECT_TASK1_HYPERCUBE_H

#include <unordered_map>
#include "../Common/bucket.h"
#include "../Common/hashFuncs.h"
#include "../Common/Distance.h"

using namespace std;

class F { // implements f mapping function of hypercube
private:
    SimpleHash h;
    unordered_map<int, char> coinFlips; // keep previous results
public:
    F(int,double,int);

    char flipTheCoin(vector<int> *);

};

class HyperCube {
private:
    vector<F *> projectors; // all mapping functions of the hypercube
    int cubeDimension;
    unordered_map<string, Bucket *> vertices;

    void splitIntoVertices(int, vector<Image *> *);
public:
    HyperCube(int, double, int, int, int, vector<Image *> *);
    virtual ~HyperCube();
    string getVertexIdx(vector<int> *);
    Bucket * getVertexByIdx(const string &);
};


#endif //PROJECT_TASK1_HYPERCUBE_H
