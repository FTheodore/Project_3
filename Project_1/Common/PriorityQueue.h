#ifndef PROJECT_TASK1_PRIORITYQUEUE_H
#define PROJECT_TASK1_PRIORITYQUEUE_H

#include <queue>
#include <tuple>
#include "image.h"
#include "Distance.h"

using namespace std;

class PriorityFurther { // used for nearest neighbour
public:
    bool operator() (tuple<int, Image *> &t1, tuple<int, Image *> &t2) {
        return get<0>(t1) < get<0>(t2);
    }
};

class PriorityCloser { // used for range search
public:
    bool operator() (tuple<int, Image *> &t1, tuple<int, Image *> &t2) {
        return get<0>(t1) > get<0>(t2);
    }
};

template<class comp>
class PriorityQueue {
private:
    priority_queue<tuple<int, Image *>,
                    vector< tuple<int, Image *> >,
                    comp> queue;

public:
    void tryInsert(Image *, Image *, int );
    void transferToVector(vector<tuple<int, Image*>> *);
    void insert(Image *, int);

};

#endif //PROJECT_TASK1_PRIORITYQUEUE_H
