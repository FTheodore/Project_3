#include <iostream>
#include <tuple>
#include <chrono>

#include "../Common/CmdArgumentsReader.h"
#include "../Common/dataset.h"
#include "../Algorithms/ExactNN.h"
#include "../Added_files/Labels.h"
#include "../Added_files/Emd.h"

#define NNEIGHBOURS 10
#define CLUSTER_ROWS 7 // 4x4 clusters i.e. 16 per image
#define CLUSTER_COLS 7

using namespace std;
using namespace std::chrono;

int main(int argc, char const *argv[]) {
    // tty or cin arguments needed for the search
    SearchEmdArguments args(argc, (char **)argv);

    // get images and labels from files
    Dataset inputFile(args.inputFile, 1);
    Dataset queryFile(args.queryFile, 1);
    Labels labels(args.inputLblsFile, args.queryLblsFile);

    //Open output file
    ofstream outputFile;
    outputFile.open(args.outputFile);
    if (!outputFile.is_open()) {
        throw runtime_error("File " + string(args.outputFile) + " cannot be opened.");
    }

    operations_research::emd(inputFile.getImages()->at(0)->getPixels(),
        queryFile.getImages()->at(0)->getPixels(),
        inputFile.getRows(),inputFile.getCols(),7,7);

    tuple<vector<tuple<double,Image*>>, microseconds> emdResults;
    tuple<vector<tuple<double,Image*>>, microseconds> manhResults;

    vector<double> emdPreds;
    vector<double> manhPreds;

    // Perform nearest neighbours search
    for (int i = 0; i < queryFile.getImageNum(); ++i) {
        //Run exactNN algorithm using Earth Mover's Distance
        emdResults = exactNN(queryFile.getImages()->at(i),
                             inputFile.getImages(),
                             NNEIGHBOURS, true,
                             inputFile.getRows(),inputFile.getCols(), CLUSTER_ROWS, CLUSTER_COLS);

        //Run exactNN algorithm using Manhattan Distance
        manhResults = exactNN(queryFile.getImages()->at(i),
                                          inputFile.getImages(),
                                          NNEIGHBOURS);

        // get percentage of correctly predicted images
        emdPreds.push_back(labels.correctPredictions(queryFile.getImages()->at(i), &get<0>(emdResults)));
        manhPreds.push_back(labels.correctPredictions(queryFile.getImages()->at(i), &get<0>(manhResults)));
    }

    double emdPredsAvg = std::accumulate(emdPreds.begin(),emdPreds.end(), 0.0) / emdPreds.size();
    double manhPredsAvg = std::accumulate(manhPreds.begin(),manhPreds.end(), 0.0) / manhPreds.size();

    outputFile << "Average Correct Search Results EMD: " << emdPredsAvg << endl;
    outputFile << "Average Correct Search Results MANHATTAN: " << manhPredsAvg << endl;


    outputFile.close();
    return 0;
}
