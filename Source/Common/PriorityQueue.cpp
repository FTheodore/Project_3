#include "PriorityQueue.h"

//insert new image into queue only if top element has a longer distance
template<class comp>
void PriorityQueue<comp>::tryInsert(Image *queryImg, Image *newImg, int numNeighbors, bool useEmd,
        int imgRows, int imgCols, int clstRows, int clstCols) {
    double newDist = useEmd ?
            operations_research::emd(queryImg->getPixels(),newImg->getPixels(), imgRows, imgCols, clstRows, clstCols)
            :manhattanDistance(queryImg->getPixels(), newImg->getPixels());
    if(this->queue.size() < numNeighbors)
        this->queue.push(make_tuple(newDist, newImg));
    else if(newDist < get<0>(queue.top())) {
        this->queue.pop();
        this->queue.push(make_tuple(newDist, newImg));
    }
}

template<class comp>
void PriorityQueue<comp>::insert(Image *newImg, double newDist) {
    this->queue.push(make_tuple(newDist, newImg));
}

template<class comp>
void PriorityQueue<comp>::transferToVector(vector<tuple<double, Image *>> * vec) {
    while(!queue.empty()) {
        vec->push_back(queue.top());
        queue.pop();
    }
}

template class PriorityQueue<PriorityFurther>;
template class PriorityQueue<PriorityCloser>;
