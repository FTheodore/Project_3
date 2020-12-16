#include "ExactNN.h"

tuple<vector<tuple<double,Image*>>, microseconds> exactNN(Image* queryImage,
                                                       vector<Image *> *datasetImages,
                                                       int numNeighbors, bool useEmd,
                                                       int imgRows, int imgCols, int clstRows, int clstCols){
    PriorityQueue<PriorityFurther> queue;

    //Start timer
    high_resolution_clock::time_point startTimer = high_resolution_clock::now();

    //find all NNeighbours and save them in a priority queue
    for(int i = 0; i < datasetImages->size(); ++i)
        queue.tryInsert(queryImage,datasetImages->at(i),numNeighbors,useEmd,imgRows,imgCols,clstRows,clstCols);

    //stop timer
    high_resolution_clock::time_point stopTimer = high_resolution_clock::now();
    auto timerDuration = duration_cast<microseconds>(stopTimer - startTimer);

    //gather results
    vector<tuple<double, Image*>> result;
    queue.transferToVector(&result);
    return make_tuple(result, timerDuration);
}
