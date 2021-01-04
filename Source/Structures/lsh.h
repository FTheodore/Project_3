#ifndef PROJECT_TASK1_LSH_H
#define PROJECT_TASK1_LSH_H

#include <vector>
#include <unordered_map>
#include "../Common/image.h"
#include "../Common/hashFuncs.h"
#include "../Common/bucket.h"

using namespace std;

class LshTable {
private:
    int tableSize;
    AmplifiedHash gHash;    //Hash function of the current table
    unordered_map<int, Bucket *> *table; //Pointers to buckets of lsh table
    void splitIntoBuckets(int, vector<Image *> *);
public:
    LshTable(int, vector<Image *> *, int, double, int );
    virtual ~LshTable();
    tuple<unsigned int, Bucket* > getBucket(vector<int> *);
};

class Lsh {
private:
    int numTables;  //Number of lsh tables
    vector<LshTable *> *tables; //Pointers to lsh tables
    void buildTables(int, vector<Image *> *, int, double, int);
public:
    Lsh(int,int, vector<Image *> *, int, double, int);
    virtual ~Lsh();
    LshTable* getHashTable(int);
    int getNumTables();
};

#endif //PROJECT_TASK1_LSH_H
