#ifndef PROJECT_TASK1_CONFIG_H
#define PROJECT_TASK1_CONFIG_H

#include <string>
#include <fstream>

#define NUM_CLUSTERS 10
#define HASH_TABLES_DEF 3
#define HASH_FUNCTIONS_DEF 4
#define IMGS_THRESH_DEF 10
#define CUBE_DIM_DEF 3
#define PROBES_DEF 2


using namespace std;

class Config {
public:
    int numClusters = NUM_CLUSTERS;
    int numHashTables = HASH_TABLES_DEF ;
    int numHashFunctions = HASH_FUNCTIONS_DEF;
    int imgsThresh = IMGS_THRESH_DEF;
    int cubeDim = CUBE_DIM_DEF;
    int probes = PROBES_DEF;
};

Config* extractConfInfo(string &filename);

#endif //PROJECT_TASK1_CONFIG_H
