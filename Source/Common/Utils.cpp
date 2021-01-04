#include "Utils.h"

// output range search results
void printRangeNrstImages(vector<tuple<double,Image*>> &apprRangeSrchImages, ofstream& outputFile) {
    vector<tuple<double,Image*>> &vec = apprRangeSrchImages;
    outputFile << "R-near neighbors: " << endl;
    for(int i = 0; i < vec.size(); ++i) {
        outputFile << "\t" << get<1>(vec.at(i))->getId() << endl;
    }
    outputFile << "~~~~~~~~~~~" << endl << endl;
}

string distanceOutput(bool isLsh) {
    return isLsh ? "distanceLSH: " : "distanceHypercube: ";
}

// output approximate nearest neighbour and exactNN results
void printResults(tuple<vector<tuple<double,Image*>>, microseconds> &apprNearestImages,
                  tuple<vector<tuple<double,Image*>>, microseconds> &exactNearestImages,
                  vector<tuple<double,Image*>> &apprRangeSrchImages,
                  Image * queryImg,
                  bool isLsh,
                  ofstream& outputFile) {
    vector<tuple<double,Image*>> &vAppr = get<0>(apprNearestImages);
    vector<tuple<double,Image*>> &vExact = get<0>(exactNearestImages);

    outputFile << "Query: " << queryImg->getId() << endl;
    int diff = (int)vExact.size() - (int)vAppr.size();
    for (int j = (int)vExact.size()-1; j >= 0; --j) {
        if(j - diff >= 0) {
            outputFile << "Approximate Nearest neighbour-"<< vExact.size() - j
                 << ": " << get<1>(vAppr.at(j - diff))->getId() << endl;
            outputFile << "Exact Nearest neighbour-"<< vExact.size() - j
                 << ": " << get<1>(vExact.at(j))->getId() << endl;

            outputFile << distanceOutput(isLsh) << (int)get<0>(vAppr.at(j - diff)) << endl;
            outputFile << "distanceTrue: " << (int)get<0>(vExact.at(j)) << endl << endl;
        }
        else {
            outputFile << "Approximate Nearest neighbour-"<< vExact.size() - j
                 << ": NOT FOUND" << endl;
            outputFile << "Exact Nearest neighbour-"<< vExact.size() - j
                 << ": " << get<1>(vExact.at(j))->getId() << endl;

            outputFile << distanceOutput(isLsh) << "-"<< endl;
            outputFile << "distanceTrue: " << (int)get<0>(vExact.at(j)) << endl << endl;
        }

    }
    string timeMessg = isLsh ? "tLSH: " : "tHypercube: ";
    outputFile << timeMessg << get<1>(apprNearestImages).count() / 1e6 << "s" << endl;
    outputFile << "tTrue: " << get<1>(exactNearestImages).count() / 1e6 << "s" << endl << endl;

    printRangeNrstImages(apprRangeSrchImages, outputFile);
}

void printComparison(tuple<vector<tuple<double,Image*>>, microseconds> &exactOldSpcNearestImage,
                     tuple<vector<tuple<double,Image*>>, microseconds> &exactNewSpcNearestImage,
                     tuple<vector<tuple<double,Image*>>, microseconds> &lshOldSpcNearestImage,
                     int exactNewOrigDist,
                     Image * queryImg,
                     bool isLsh,
                     ofstream& outputFile) {
    vector<tuple<double,Image*>> &vApprOld = get<0>(lshOldSpcNearestImage);
    vector<tuple<double,Image*>> &vExactNew = get<0>(exactNewSpcNearestImage);
    vector<tuple<double,Image*>> &vExactOld = get<0>(exactOldSpcNearestImage);

    outputFile << "Query: " << queryImg->getId() << endl;

    outputFile << "Nearest Neighbour Reduced: "
               << get<1>(vExactNew.at(0))->getId() << endl;
    if(!vApprOld.empty()) {
        outputFile << "Nearest Neighbour LSH: "
                   << get<1>(vApprOld.at(0))->getId() << endl;
    }
    else {
        outputFile << "Nearest Neighbour LSH: NOT FOUND" << endl;
    }
    outputFile << "Nearest Neighbour True: "
        << get<1>(vExactOld.at(0))->getId() << endl << endl;


    outputFile << "distanceReduced: (Reduced) " << (int)get<0>(vExactNew.at(0))
               << ", (Original) " << exactNewOrigDist << endl;
    if(!vApprOld.empty()) {
        outputFile << distanceOutput(isLsh) << (int)get<0>(vApprOld.at(0)) << endl;
    }
    else {
        outputFile << distanceOutput(isLsh) << "-" << endl;
    }
    outputFile << "distanceTrue: " << (int)get<0>(vExactOld.at(0)) << endl << endl;


    outputFile << "tReduced: " << get<1>(exactNewSpcNearestImage).count() / 1e6 << "s" << endl;
    outputFile << "tLSH: " << get<1>(lshOldSpcNearestImage).count() / 1e6 << "s" << endl;
    outputFile << "tTrue: " << get<1>(exactOldSpcNearestImage).count() / 1e6 << "s" << endl << endl;

}

void approxFactor(vector<double> apprFactorTermsLSH,
                  vector<double> apprFactorTermsReduced,
                  ofstream& outputFile) {
    // calculate means
    double apprFacLSH = std::accumulate(apprFactorTermsLSH.begin(), apprFactorTermsLSH.end(), 0.0);
    apprFacLSH /= apprFactorTermsLSH.size();

    double apprFacReduced = std::accumulate(apprFactorTermsReduced.begin(), apprFactorTermsReduced.end(), 0.0);
    apprFacReduced /= apprFactorTermsReduced.size();

    outputFile << "Approximation Factor LSH: " << apprFacLSH << endl;
    outputFile << "Approximation Factor Reduced: " << apprFacReduced << endl;
}

void unmarkImgs(vector<Image*> * imgs, int imgNum) {
    for (int i = 0; i < imgNum; ++i)
        imgs->at(i)->unmarkImage();
}

void unassignImgs(vector<Image*> * imgs, int imgNum) {
    for (int i = 0; i < imgNum; ++i)
        imgs->at(i)->unassignImageFromClst();
}

void getNearbyVertices(vector<string> &vec,
                       string currentVertex,
                       int i,
                       int changesLeft) {
    if(changesLeft == 0) { // hamming distance reached
        vec.push_back(currentVertex);
        return;
    }
    if(i < 0) // no more bits to flip
        return;
    //flip current bit
    currentVertex[i] = currentVertex[i] == '0' ? '1' : '0';
    getNearbyVertices(vec, currentVertex, i - 1, changesLeft - 1);
    //undo flip
    currentVertex[i] = currentVertex[i] == '0' ? '1' : '0';
    getNearbyVertices(vec, currentVertex, i - 1, changesLeft);
}

// get neighbour vertices in ascending hamming order
void getVerticesToCheck(vector<string> &vec,
                        string &currentVertex,
                        int maxDistance) {
    for(int curDist = 0; curDist <= maxDistance; ++curDist)
        getNearbyVertices(vec, currentVertex, (int)currentVertex.length() - 1, curDist);
}

// read parameter values from custom made config file to avoid recompilation
void readParams(int & w_smpl_prcnt, int & w_factor,
                bool readApproxThresh, int * approx_threshold,
                bool readClstThresh, int * clust_threshold,
                bool readClstThreshIters, int * clust_iters) {
    string fileName;
    cout << "Insert path of parameters config file: ";
    cin >> fileName;
    cout << endl;
    ifstream inpFile(fileName);

    if(!inpFile.is_open())
        throw runtime_error("File " + fileName + " cannot be opened.");

    string curLine;

    while(getline(inpFile, curLine)) {
        int idx = curLine.find(": ");
        if(curLine.substr(0, idx) == "w_smpl_prcnt") {
            w_smpl_prcnt = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "w_factor") {
            w_factor = stoi(curLine.substr(idx+1));
        }
        else if(readApproxThresh && curLine.substr(0, idx) == "approx_threshold") {
            *approx_threshold = stoi(curLine.substr(idx+1));
        }
        else if(readClstThresh && curLine.substr(0, idx) == "clust_threshold") {
            *clust_threshold = stoi(curLine.substr(idx+1));
        }
        else if(readClstThreshIters && curLine.substr(0, idx) == "clust_threshold_iters") {
            *clust_iters = stoi(curLine.substr(idx+1));
        }
    }

    inpFile.close();
}