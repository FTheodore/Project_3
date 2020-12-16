#ifndef PROJECT_TASK1_PRIORITYQUEUE_H
#define PROJECT_TASK1_PRIORITYQUEUE_H

#include <queue>
#include <tuple>
#include "image.h"
#include "Distance.h"
#include "../Added_files/Emd.h"

using namespace std;

class PriorityFurther { // used for nearest neighbour
public:
    bool operator() (tuple<double, Image *> &t1, tuple<double, Image *> &t2) {
        return get<0>(t1) < get<0>(t2);
    }
};

class PriorityCloser { // used for range search
public:
    bool operator() (tuple<double, Image *> &t1, tuple<double, Image *> &t2) {
        return get<0>(t1) > get<0>(t2);
    }
};

template<class comp>
class PriorityQueue {
private:
    priority_queue<tuple<double, Image *>,
                    vector< tuple<double, Image *> >,
                    comp> queue;

public:
    void tryInsert(Image *queryImg, Image *newImg, int numNeighbors, bool=false,
                   int=0, int=0, int=0, int=0);
    void transferToVector(vector<tuple<double, Image*>> *);
    void insert(Image *, double);

};

#endif //PROJECT_TASK1_PRIORITYQUEUE_H
