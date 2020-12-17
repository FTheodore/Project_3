#include "CmdArgumentsReader.h"

using namespace std;

//Functions that read command line arguments

LshCmdVariables* setLshArguments(int argc, char const *argv[])
{
    LshCmdVariables* lshCmdVariables = new LshCmdVariables;
    for(int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i], "-D") == 0)
        {
            lshCmdVariables->inputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-q") == 0 || strcmp(argv[i], "-Q") == 0)
        {
            lshCmdVariables->queryFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-k") == 0 || strcmp(argv[i], "-K") == 0)
        {
            lshCmdVariables->numHashFuncts = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-l") == 0 || strcmp(argv[i], "-L") == 0)
        {
            lshCmdVariables->lshTables = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            lshCmdVariables->outputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-n") == 0 || strcmp(argv[i], "-N") == 0)
        {
            lshCmdVariables->numNN = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-r") == 0 || strcmp(argv[i], "-R") == 0)
        {
            lshCmdVariables->radius = stof(argv[i+1]);
        }
    }
    return lshCmdVariables;
}

CubeCmdVariables* setCubeArguments(int argc, char const *argv[])
{
    CubeCmdVariables* cubeCmdVariables = new CubeCmdVariables;
    for(int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i], "-D") == 0)
        {
            cubeCmdVariables->inputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-q") == 0 || strcmp(argv[i], "-Q") == 0)
        {
            cubeCmdVariables->queryFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-k") == 0 || strcmp(argv[i], "-K") == 0)
        {
            cubeCmdVariables->cubeDim = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-m") == 0 || strcmp(argv[i], "-M") == 0)
        {
            cubeCmdVariables->imgsThresh = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-probes") == 0 || strcmp(argv[i], "-PROBES") == 0)
        {
            cubeCmdVariables->probes = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            cubeCmdVariables->outputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-n") == 0 || strcmp(argv[i], "-N") == 0)
        {
            cubeCmdVariables->numNN = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-r") == 0 || strcmp(argv[i], "-R") == 0)
        {
            cubeCmdVariables->radius = stof(argv[i+1]);
        }
    }
    return cubeCmdVariables;
}

ClusterCmdVariables* setClusterArguments(int argc, char const *argv[])
{
    ClusterCmdVariables* clusterCmdVariables = new ClusterCmdVariables;
    for(int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i],"-i") == 0 || strcmp(argv[i], "-I") == 0)
        {
            clusterCmdVariables->inputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i], "-C") == 0)
        {
            clusterCmdVariables->configFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            clusterCmdVariables->outputFileName = argv[i+1];
        }
        else if(strcmp(argv[i],"-complete") == 0 || strcmp(argv[i], "-COMPLETE") == 0)
        {
            clusterCmdVariables->complete = true;
        }
        else if(strcmp(argv[i],"-m") == 0 || strcmp(argv[i], "-M") == 0)
        {
            clusterCmdVariables->method = argv[i+1];
        }
    }
    return clusterCmdVariables;
}


void SearchRdcedArguments::getFromTTY(const int & argc, char ** argv) {
    for(int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i], "-D") == 0)
        {
            this->dataFileOldSpace = argv[i+1];
        }
        else if(strcmp(argv[i],"-i") == 0 || strcmp(argv[i], "-I") == 0)
        {
            this->dataFileNewSpace = argv[i+1];
        }
        else if(strcmp(argv[i],"-q") == 0 || strcmp(argv[i], "-Q") == 0)
        {
            this->queryFileOldSpace = argv[i+1];
        }
        else if(strcmp(argv[i],"-s") == 0 || strcmp(argv[i], "-S") == 0)
        {
            this->queryFileNewSpace = argv[i+1];
        }
        else if(strcmp(argv[i],"-k") == 0 || strcmp(argv[i], "-K") == 0)
        {
            this->numHashFuncts = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-l") == 0 || strcmp(argv[i], "-L") == 0)
        {
            this->lshTables = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            this->outputFile = argv[i+1];
        }
    }
}
void SearchRdcedArguments::getFromUser() {
    if(this->dataFileOldSpace.empty()) {
        cout << "Insert path of input file of original space: ";
        cin >> this->dataFileOldSpace;
        cout << endl;
    }

    if(this->dataFileNewSpace.empty()) {
        cout << "Insert path of input file of new space: ";
        cin >> this->dataFileNewSpace;
        cout << endl;
    }

    if(this->queryFileOldSpace.empty()) {
        cout << "Insert path of query file of original space: ";
        cin >> this->queryFileOldSpace;
        cout << endl;
    }

    if(this->queryFileNewSpace.empty()) {
        cout << "Insert path of query file of new space: ";
        cin >> this->queryFileNewSpace;
        cout << endl;
    }

    if(this->outputFile.empty()) {
        cout << "Insert path of output file: ";
        cin >> this->outputFile;
        cout << endl;
    }
}

SearchRdcedArguments::SearchRdcedArguments(const int & argc, char ** argv) {
    this->getFromTTY(argc, argv);
    this->getFromUser();
}


void SearchEmdArguments::getFromTTY(const int & argc, char ** argv) {
    for(int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i], "-D") == 0)
        {
            this->inputFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-q") == 0 || strcmp(argv[i], "-Q") == 0)
        {
            this->queryFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-l1") == 0 || strcmp(argv[i], "-L1") == 0)
        {
            this->inputLblsFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-l2") == 0 || strcmp(argv[i], "-L2") == 0)
        {
            this->queryLblsFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            this->outputFile = argv[i+1];
        }
    }
}
void SearchEmdArguments::getFromUser() {
    if(this->inputFile.empty()) {
        cout << "Insert path of input file of original space: ";
        cin >> this->inputFile;
        cout << endl;
    }

    if(this->queryFile.empty()) {
        cout << "Insert path of query file of original space: ";
        cin >> this->queryFile;
        cout << endl;
    }

    if(this->inputLblsFile.empty()) {
        cout << "Insert path of input labels file: ";
        cin >> this->inputLblsFile;
        cout << endl;
    }

    if(this->queryLblsFile.empty()) {
        cout << "Insert path of query labels file: ";
        cin >> this->queryLblsFile;
        cout << endl;
    }

    if(this->outputFile.empty()) {
        cout << "Insert path of output file: ";
        cin >> this->outputFile;
        cout << endl;
    }
}

SearchEmdArguments::SearchEmdArguments(const int & argc, char ** argv) {
    this->getFromTTY(argc, argv);
    this->getFromUser();
}


void ClusteringArguments::getFromTTY(const int & argc, char ** argv) {
    for(int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i], "-D") == 0)
        {
            this->inputFileOriginalSpc = argv[i+1];
        }
        else if(strcmp(argv[i],"-i") == 0 || strcmp(argv[i], "-I") == 0)
        {
            this->inputFileNewSpc = argv[i+1];
        }
        else if(strcmp(argv[i],"-n") == 0 || strcmp(argv[i], "-N") == 0)
        {
            this->clustersFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i], "-C") == 0)
        {
            this->configFile = argv[i+1];
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i], "-O") == 0)
        {
            this->outputFile = argv[i+1];
        }
    }
}
void ClusteringArguments::getFromUser() {
    if(this->inputFileOriginalSpc.empty()) {
        cout << "Insert path of input file of original space: ";
        cin >> this->inputFileOriginalSpc;
        cout << endl;
    }

    if(this->inputFileNewSpc.empty()) {
        cout << "Insert path of input file of new space: ";
        cin >> this->inputFileNewSpc;
        cout << endl;
    }

    if(this->clustersFile.empty()) {
        cout << "Insert path of clusters file: ";
        cin >> this->clustersFile;
        cout << endl;
    }

    if(this->configFile.empty()) {
        cout << "Insert path of configuration file: ";
        cin >> this->configFile;
        cout << endl;
    }

    if(this->outputFile.empty()) {
        cout << "Insert path of output file: ";
        cin >> this->outputFile;
        cout << endl;
    }
}

ClusteringArguments::ClusteringArguments(const int & argc, char ** argv) {
    this->getFromTTY(argc, argv);
    this->getFromUser();
}