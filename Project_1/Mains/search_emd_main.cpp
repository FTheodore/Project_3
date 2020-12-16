#include <iostream>
#include <tuple>
#include <chrono>

#include "../Common/CmdArgumentsReader.h"
#include "../Common/dataset.h"
#include "../Added_files/Labels.h"
#include "../Added_files/Emd.h"

#define NNEIGHBOURS 10

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

    emd(inputFile.getImages()->at(0)->getPixels(),
        queryFile.getImages()->at(0)->getPixels(),
        inputFile.getRows(),inputFile.getCols(),7,7);

    outputFile.close();
    return 0;
}
