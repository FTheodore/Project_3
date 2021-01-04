#include "Config.h"

//Read cluster parameters from cluster.conf file
Config* extractConfInfo(string &filename) {
    Config* conf = new Config();
    ifstream inpFile(filename);
    if(!inpFile.is_open())
        throw runtime_error("File " + filename + " cannot be opened.");

    string curLine;

    while(getline(inpFile, curLine)) {
        int idx = curLine.find(": ");
        if(curLine.substr(0, idx) == "number_of_clusters") {
            conf->numClusters = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "number_of_vector_hash_tables") {
            conf->numHashTables = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "number_of_vector_hash_functions") {
            conf->numHashFunctions = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "max_number_M_hypercube") {
            conf->imgsThresh = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "number_of_hypercube_dimensions") {
            conf->cubeDim = stoi(curLine.substr(idx+1));
        }
        else if(curLine.substr(0, idx) == "number_of_probes") {
            conf->probes = stoi(curLine.substr(idx+1));
        }
    }

    inpFile.close();
    return conf;
}
