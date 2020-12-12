#ifndef PROJECT_TASK1_CMDARGUMENTSREADER_H
#define PROJECT_TASK1_CMDARGUMENTSREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

//LSH DEFAULTS DEFINED
#define K_LSH_DEFAULT 4
#define L_LSH_DEFAULT 5
#define N_LSH_DEFAULT 1
#define R_LSH_DEFAULT 10000

//CUBE DEFAULTS DEFINED
#define K_CUBE_DEFAULT 14
#define M_CUBE_DEFAULT 10
#define PROBES_CUBE_DEFAULT 2
#define N_CUBE_DEFAULT 1
#define R_CUBE_DEFAULT 10000

using namespace std;

class LshCmdVariables {
public:
    string inputFileName;   //input file
    string queryFileName;   //query file
    int numHashFuncts = K_LSH_DEFAULT;
    int lshTables = L_LSH_DEFAULT;
    string outputFileName;  //output file
    int numNN = N_LSH_DEFAULT;  // number of nearest
    double radius = R_LSH_DEFAULT;  //radius
};

class CubeCmdVariables {
public:
    string inputFileName;   //input file
    string queryFileName;   //query file
    int cubeDim = K_CUBE_DEFAULT;
    int imgsThresh = M_CUBE_DEFAULT;
    int probes = PROBES_CUBE_DEFAULT;
    string outputFileName;  //output file
    int numNN = N_CUBE_DEFAULT; //number of nearest
    double radius = R_CUBE_DEFAULT;  //radius
};

class ClusterCmdVariables {
public:
    string inputFileName;   //input file
    string configFileName;  //configuration file
    string outputFileName;  //output file
    bool complete = false;          //complete
    string method;          //Method: Classic(Lloyds) or LSH or Hypercube
};

LshCmdVariables* setLshArguments(int argc, char const *argv[]);
CubeCmdVariables* setCubeArguments(int argc, char const *argv[]);
ClusterCmdVariables* setClusterArguments(int argc, char const *argv[]);


#endif //PROJECT_TASK1_CMDARGUMENTSREADER_H
