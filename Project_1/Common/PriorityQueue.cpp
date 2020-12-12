#include "PriorityQueue.h"

//insert new image into queue only if top element has a longer distance
template<class comp>
void PriorityQueue<comp>::tryInsert(Image *queryImg, Image *newImg, int numNeighbors) {
    int newDist = manhattanDistance(queryImg->getPixels(), newImg->getPixels());
    if(this->queue.size() < numNeighbors)
        this->queue.push(make_tuple(newDist, newImg));
    else if(newDist < get<0>(queue.top())) {
        this->queue.pop();
        this->queue.push(make_tuple(newDist, newImg));
    }
}

template<class comp>
void PriorityQueue<comp>::insert(Image *newImg, int newDist) {
    this->queue.push(make_tuple(newDist, newImg));
}

template<class comp>
void PriorityQueue<comp>::transferToVector(vector<tuple<int, Image *>> * vec) {
    while(!queue.empty()) {
        vec->push_back(queue.top());
        queue.pop();
    }
}

template class PriorityQueue<PriorityFurther>;
template class PriorityQueue<PriorityCloser>;
