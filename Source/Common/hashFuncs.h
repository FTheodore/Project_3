//
// Created by sysgod on 08/10/2020.
//

#ifndef PROJECT_TASK1_HASHFUNCS_H
#define PROJECT_TASK1_HASHFUNCS_H

#include "image.h"
#include <cmath>
#include <random>
#include <chrono>
#include "Utils.h"

using namespace std;
using namespace std::chrono;

class SimpleHash { // h hash function from lectures
private:
    int dimension; // image vector size
    double gridW;
    int divisor; // M from slides [2^(32/k)]
    vector<double> *shifts; // s_i disturbances
    void randShifts();
    static int mod(int,int);
    static int modularExp(unsigned int,unsigned int,int);
public:
    SimpleHash(int,double,int);
    ~SimpleHash();
    double generateDcml() const;
    int hashResult(vector<int>*);
    static int mod(unsigned int,int);
};

class AmplifiedHash { // g hash function from lectures
private:
    int tableSize; // size of hash table current g function is associated with
    int numHashes; // # of h_i functions
    vector<SimpleHash *> *subhashes;

public:
    AmplifiedHash(int, double, int, int);
    virtual ~AmplifiedHash();
    tuple<int,unsigned int> hashResult(vector<int> *);
};

#endif //PROJECT_TASK1_HASHFUNCS_H
