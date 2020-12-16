#include <iostream>
#include <tuple>
#include <chrono>


#include "../Common/CmdArgumentsReader.h"
#include "../Common/dataset.h"
#include "../Common/hashFuncs.h"
#include "../Algorithms/AproxNN.h"
#include "../Algorithms/RangeSearch.h"


using namespace std;
using namespace std::chrono;


int main(int argc, char const *argv[]) {
    LshCmdVariables *lshCmdVariables = setLshArguments(argc, argv);
    bool termination;

    do {
        //Ask from user the path of dataset
        if (lshCmdVariables->inputFileName.empty()) {
            cout << "Insert path of dataset file: ";
            cin >> lshCmdVariables->inputFileName;
            cout << endl;
        }

        Dataset inputFile(lshCmdVariables->inputFileName, 1);

        int w_smpl_prcnt, w_factor, approx_threshold;
        readParams(w_smpl_prcnt, w_factor, true, &approx_threshold);
        //Structures creation here
        double W = calcW(inputFile.getImages(),w_smpl_prcnt, inputFile.getImageNum());
//        cout << "W: " << W*w_factor << endl;
        cout << "Building Lsh Structure..." << endl;
        Lsh lsh(lshCmdVariables->lshTables, inputFile.getImageNum(), inputFile.getImages(),
                inputFile.getDimensions(), w_factor*W, lshCmdVariables->numHashFuncts);
        cout << "Done" << endl;
        //Ask from user the path of query file
        if (lshCmdVariables->queryFileName.empty()) {
            cout << "Insert path of query file: ";
            cin >> lshCmdVariables->queryFileName;
            cout << endl;
        }

        Dataset queryFile(lshCmdVariables->queryFileName, 1);

        //Ask from user the path of output file
        if (lshCmdVariables->outputFileName.empty()) {
            cout << "Insert path of output file: ";
            cin >> lshCmdVariables->outputFileName;
            cout << endl;
        }

        //Open output file
        ofstream outputFile;
        outputFile.open(lshCmdVariables->outputFileName);
        if (!outputFile.is_open()) {
            throw runtime_error("File " + string(lshCmdVariables->outputFileName) + " cannot be opened.");
        }

        //Nearest image tuple -> contains imagePtr, distance and total time of calculation
        tuple<vector<tuple<double,Image*>>, microseconds> exactNearestImages;
        tuple<vector<tuple<double,Image*>>, microseconds> apprNearestImages;
        vector<tuple<double,Image*>> apprRangeSrchImages;
        for(int i = 0; i < queryFile.getImages()->size(); i++) {
            //Run exactNN algorithm
            exactNearestImages = exactNN(queryFile.getImages()->at(i),
                                        inputFile.getImages(),
                                        lshCmdVariables->numNN);

            //Run approximateNN algorithm
            apprNearestImages = aproxKNN(queryFile.getImages()->at(i),
                                         &lsh,
                                         lshCmdVariables->numNN, approx_threshold);

            //Clear previously marked images from approximateNN
            unmarkImgs(inputFile.getImages(),inputFile.getImageNum());

            //Run approximate range search algorithm
            apprRangeSrchImages = aproxRangeSrch(queryFile.getImages()->at(i)->getPixels(),
                                               &lsh,
                                               lshCmdVariables->radius, approx_threshold);

            //Clear previously marked images for next query
            unmarkImgs(inputFile.getImages(),inputFile.getImageNum());

            //Print the algorithms results
            printResults(apprNearestImages,
                         exactNearestImages,
                         apprRangeSrchImages,
                         queryFile.getImages()->at(i),
                         true,
                         outputFile); // bool affects output message
        }

        outputFile.close();
        //Ask user if he wants to exit or do another search
        cout << "1. Do another search." << endl;
        cout << "2. Terminate program." << endl;
        cout << "Insert your choice: ";
        while(true){
            int choice;
            cin >> choice;
            if (choice == 1) {
                lshCmdVariables->inputFileName = "";
                lshCmdVariables->queryFileName = "";
                lshCmdVariables->outputFileName = "";
                termination = false;
                break;
            } else if (choice == 2) {
                termination = true;
                break;
            }
            else {
                cout << "Invalid choice, reenter: ";
            }
        }
    } while (!termination);

    delete lshCmdVariables;     //Free allocated memory
    return 0;
}
