#include "RangeSearch.h"

vector<tuple<int,Image*>> aproxRangeSrch(vector<unsigned char> * queryImage,
                                        Lsh* structure,
                                        double radius,
                                        int threshold) {
    PriorityQueue<PriorityCloser> queue;

    // number of lsh tables
    int numTables = structure->getNumTables();

    int checked = 0;
    for (int i = 0; i < numTables; ++i) { // iterate through lsh tables
        LshTable *tbl = structure->getHashTable(i);
        // hash the image and get the corresponding bucket
        tuple<unsigned int, Bucket *> bucketTpl = tbl->getBucket(queryImage);
        Bucket *buckPtr = get<1>(bucketTpl);
        unsigned int hashRes = get<0>(bucketTpl); // hash result before modulo operation was applied by g function
        if (buckPtr == nullptr) // bucket is empty
            continue;
        vector<Image *> *buckImgs = buckPtr->getImages();
        vector<unsigned int> *g_hash_results = buckPtr->getHashReslts();
        for (int j = 0; j < buckImgs->size(); ++j) {
            // images have different hash values before mod or the image has been checked in a previous LSH table
            if(buckImgs->at(j)->isMarked() || g_hash_results->at(j) != hashRes)
                continue;
            buckImgs->at(j)->markImage();

            int newDist = manhattanDistance(queryImage, buckImgs->at(j)->getPixels());
            if(newDist <= radius)
                queue.insert(buckImgs->at(j), newDist);

            if(++checked == (2*threshold*numTables))
                break;
        }
        if(checked == (2*threshold*numTables))
            break;
    }

    //gather results
    vector<tuple<int, Image*>> result;
    queue.transferToVector(&result);
    return result;
}

vector<tuple<int, Image*>> aproxRangeSrch(vector<unsigned char> * queryImage,
                                          HyperCube* structure,
                                          int checkThrshld,
                                          int maxProbes,
                                          double radius) {
    PriorityQueue<PriorityCloser> queue;

    vector<string> verticesToCheck;
    string queryVrtx = structure->getVertexIdx(queryImage);
    getVerticesToCheck(verticesToCheck, queryVrtx, queryVrtx.length());

    int checked = 0;    // stop when a lot of potential NNeighbours are checked
    for(int i = 0; i < maxProbes; ++i) {   //start probing vertices
        if(i >= verticesToCheck.size()) // no vertices left to check
            break;
        string curVrtx = verticesToCheck.at(i); // get next vertex of lowest hamming distance
        Bucket * bucketPtr = structure->getVertexByIdx(curVrtx);
        if(bucketPtr == nullptr) // vertex/bucket is empty
            continue;
        vector<Image *> *buckImgs = bucketPtr->getImages();
        for(int j = 0; j < buckImgs->size(); ++j) { // iterate through all images of current vertex
            int newDist = manhattanDistance(queryImage, buckImgs->at(j)->getPixels());
            if(newDist <= radius)
                queue.insert(buckImgs->at(j), newDist);

            if(++checked == checkThrshld) // M threshold reached, stop algorithm
                break;
        }
        if(checked == checkThrshld)
            break;
    }

    //gather results
    vector<tuple<int, Image*>> result;
    queue.transferToVector(&result);
    return result;
}
