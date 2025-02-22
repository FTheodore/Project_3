#include <iostream>
#include <tuple>
#include <chrono>


#include "../Common/CmdArgumentsReader.h"
#include "../Common/dataset.h"
#include "../Common/hashFuncs.h"
#include "../Algorithms/AproxNN.h"

#define NNEIGHBOURS 1

using namespace std;
using namespace std::chrono;

int main(int argc, char const *argv[]) {
    // tty or cin arguments needed for the search
    SearchRdcedArguments args(argc, (char **)argv);

    Dataset inputFileOldSpace(args.dataFileOldSpace, 1);
    Dataset inputFileNewSpace(args.dataFileNewSpace, 2);

    int w_smpl_prcnt, w_factor, approx_threshold;
    readParams(w_smpl_prcnt, w_factor, true, &approx_threshold);

    //Lsh Structures creation here
    double W = calcW(inputFileOldSpace.getImages(),w_smpl_prcnt, inputFileOldSpace.getImageNum());
        cout << "W old space: " << W*w_factor << endl;
    cout << "Building Lsh Structure for old space..." << endl;
    Lsh lshOldSpace(args.lshTables, inputFileOldSpace.getImageNum(), inputFileOldSpace.getImages(),
            inputFileOldSpace.getDimensions(), w_factor*W, args.numHashFuncts);
    cout << "Done" << endl;


    Dataset queryFileOldSpace(args.queryFileOldSpace, 1);
    Dataset queryFileNewSpace(args.queryFileNewSpace, 2);

    //Open output file
    ofstream outputFile;
    outputFile.open(args.outputFile);
    if (!outputFile.is_open()) {
        throw runtime_error("File " + string(args.outputFile) + " cannot be opened.");
    }

    //Nearest image tuple -> contains imagePtr, distance and total time of calculation
    tuple<vector<tuple<double,Image*>>, microseconds> exactOldSpcNearestImage;
    tuple<vector<tuple<double,Image*>>, microseconds> exactNewSpcNearestImage;
    tuple<vector<tuple<double,Image*>>, microseconds> lshOldSpcNearestImage;

    vector<double> apprFactorTermsLSH;
    vector<double> apprFactorTermsReduced;

    for(int i = 0; i < queryFileOldSpace.getImageNum(); i++) {
        //Run exactNN algorithm on old space
        exactOldSpcNearestImage = exactNN(queryFileOldSpace.getImages()->at(i),
                                     inputFileOldSpace.getImages(),
                                     NNEIGHBOURS);

        //Run exactNN algorithm on new space
        exactNewSpcNearestImage = exactNN(queryFileNewSpace.getImages()->at(i),
                                     inputFileNewSpace.getImages(),
                                     NNEIGHBOURS);
        // Find l1 distance between query img and nearest img on original space
        int imgId = get<1>(get<0>(exactNewSpcNearestImage).at(0))->getId();
        int originalSpaceDist = manhattanDistance(queryFileOldSpace.getImages()->at(i)->getPixels(),
                                                  inputFileOldSpace.getImages()->at(imgId)->getPixels());

        //Run approximateNN algorithm on old space
        lshOldSpcNearestImage = aproxKNN(queryFileOldSpace.getImages()->at(i),
                                     &lshOldSpace,
                                     NNEIGHBOURS, approx_threshold);

        //Clear previously marked images from approximateNN
        unmarkImgs(inputFileOldSpace.getImages(),inputFileOldSpace.getImageNum());

        //Print the algorithms results
        printComparison(exactOldSpcNearestImage,
                        exactNewSpcNearestImage,
                        lshOldSpcNearestImage,
                        originalSpaceDist,
                        queryFileOldSpace.getImages()->at(i),
                        true,
                        outputFile); // bool affects output message

        // save distances for approximation factor calculation
        apprFactorTermsReduced.push_back(
                (double)originalSpaceDist / get<0>(get<0>(exactOldSpcNearestImage).at(0))
                        );
        if(!get<0>(lshOldSpcNearestImage).empty())
            apprFactorTermsLSH.push_back((double)
                get<0>(get<0>(lshOldSpcNearestImage).at(0)) /
                        get<0>(get<0>(exactOldSpcNearestImage).at(0))
                    );
    }

    approxFactor(apprFactorTermsLSH, apprFactorTermsReduced, outputFile);

    outputFile.close();
    return 0;
}

