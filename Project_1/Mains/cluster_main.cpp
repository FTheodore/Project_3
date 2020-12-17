#include <iostream>
#include <tuple>
#include <chrono>

#include "../Common/CmdArgumentsReader.h"
#include "../Common/dataset.h"
#include "../Clustering/algorithm.h"
#include "../Clustering/Silhouette.h"
#include "../Clustering/Config.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char const *argv[]) {
    // tty or cin arguments needed for clustering
    ClusteringArguments args(argc, (char **)argv);

    Dataset inputFileOldSpace(args.inputFileOriginalSpc, 1);
    Dataset inputFileNewSpace(args.inputFileNewSpc, 2);

    //Extract config file info
    Config* conf = extractConfInfo(args.configFile);
    if(conf->numClusters < 1)
        throw runtime_error("Num of clusters must be > 0\n Please check "+ args.configFile);

    int clust_threshold, w_smpl_prcnt, w_factor, approx_threshold, clust_iters;
    readParams(w_smpl_prcnt,w_factor,
               true,&approx_threshold,
               true,&clust_threshold,
               true, &clust_iters);

    // perform lloyd's assignment
    const string method = "Classic";


    //Run clustering algorithm on original space
    cout << "Clustering on original space..." << endl;
    //start timer
    high_resolution_clock::time_point startTimer = high_resolution_clock::now();

    vector<Cluster *> * clustersOrigSpc = clustering(method,*inputFileOldSpace.getImages(),
                                              inputFileOldSpace.getImages(), conf->numClusters,
                                              conf->imgsThresh, conf->probes, nullptr, nullptr,
                                              approx_threshold, clust_threshold, clust_iters);

    //stop timer
    high_resolution_clock::time_point stopTimer = high_resolution_clock::now();
    auto timerDuration = duration_cast<microseconds>(stopTimer - startTimer);

    //Calc duration
    string durResultOriginalSpc = to_string(timerDuration.count() / 1e6) + "s";
    cout << "Done" << endl << endl;


    //Run clustering algorithm on new space
    cout << "Clustering on new space..." << endl;
    //start timer
    startTimer = high_resolution_clock::now();

    vector<Cluster *> * clustersNewSpc = clustering(method,*inputFileNewSpace.getImages(),
                                              inputFileNewSpace.getImages(), conf->numClusters,
                                              conf->imgsThresh, conf->probes, nullptr, nullptr,
                                              approx_threshold, clust_threshold, clust_iters);

    //stop timer
    stopTimer = high_resolution_clock::now();
    timerDuration = duration_cast<microseconds>(stopTimer - startTimer);

    //Calc duration
    string durResultNewSpc = to_string(timerDuration.count() / 1e6) + "s";
    cout << "Done" << endl << endl;

    cout << "Creating clusters from classification..." << endl;
    vector<Cluster *> * clustersClassify = makeClustersFromFile(inputFileOldSpace.getImages(),
                                                                args.clustersFile,
                                                                inputFileOldSpace.getDimensions());
    cout << "Done" << endl << endl;

    cout << "Calculating Silhouettes..." << endl;

    //Calculate silhouettes
    vector<double> silhouetteResOrig = silhouette(*clustersOrigSpc);

    vector<double> silhouetteResNew = silhouette(*clustersNewSpc);

    vector<double> silhouetteResClass = silhouette(*clustersClassify);

    cout << "Done" << endl << endl;

    //Free allocated memory
    for (Cluster* const &clst: *clustersOrigSpc) {
        delete clst;
    }
    for (Cluster* const &clst: *clustersNewSpc) {
        delete clst;
    }
    delete clustersOrigSpc;
    delete clustersNewSpc;
    delete conf;

    return 0;
}