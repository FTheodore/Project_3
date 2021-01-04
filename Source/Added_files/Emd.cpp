#include "Emd.h"

double operations_research::emd(vector<int> * firstImg, vector<int> * secondImg, int imageRows, int imageCols, int clstRows, int clstCols)
{
    //Construct the clusters
    ImgClusters firstImgClstrs(firstImg, imageRows, imageCols, clstRows, clstCols);
    ImgClusters secondImgClstrs(secondImg, imageRows, imageCols, clstRows, clstCols);

    //Save flows
    unordered_map<string, MPVariable* const> flows;

    //Save distances (d_i)
    unordered_map<string, const double> distances;

    // Create the linear solver with the GLOP backend.
    MPSolver* solver = MPSolver::CreateSolver("GLOP");

    //Upper bound
    const double infinity = solver->infinity();

    //Define flows and calculate distances between clusters
    for(int i = 0; i < firstImgClstrs.getClusters()->size(); ++i)
    {
        for(int j = 0; j < secondImgClstrs.getClusters()->size(); ++j)
        {
            //Create f_ij
            MPVariable* const flow = solver->MakeNumVar(0.0, infinity, "f_"+std::to_string(i)+std::to_string(j));
            //Calculate euclidean distance between clusters
            const double euclDist = euclideanDist(get<0>(firstImgClstrs.getClusters()->at(i)),
                    get<0>(secondImgClstrs.getClusters()->at(j)));

            flows.insert(make_pair(std::to_string(i)+std::to_string(j),flow));
            distances.insert(make_pair(std::to_string(i)+std::to_string(j),euclDist));
        }
    }

    // Sum of Extrinsic flows must be equal to weight
    for(int i = 0; i < firstImgClstrs.getClusters()->size(); ++i)
    {
        string constName = "ci_"+std::to_string(i);
        MPConstraint* const constrain = solver->MakeRowConstraint(get<1>(firstImgClstrs.getClusters()->at(i)),
                                                                  get<1>(firstImgClstrs.getClusters()->at(i)),
                                                                  constName);
        for(int j = 0; j < secondImgClstrs.getClusters()->size(); ++j)
        {
            constrain->SetCoefficient(flows.at(std::to_string(i)+std::to_string(j)),1);
        }
    }


    // Sum of Intrinsic flows must be equal to weight
    for(int j = 0; j < secondImgClstrs.getClusters()->size(); ++j)
    {
        string constName = "cj_"+std::to_string(j);
        MPConstraint* const constrain = solver->MakeRowConstraint(get<1>(secondImgClstrs.getClusters()->at(j)),
                                                                  get<1>(secondImgClstrs.getClusters()->at(j)),
                                                                  constName);
        for(int i = 0; i < firstImgClstrs.getClusters()->size(); ++i)
        {
            constrain->SetCoefficient(flows.at(std::to_string(i)+std::to_string(j)),1);
        }
    }


    // Define Objective function
    MPObjective* const objective = solver->MutableObjective();

    for(int i = 0; i < firstImgClstrs.getClusters()->size(); ++i)
    {
        for(int j = 0; j < secondImgClstrs.getClusters()->size(); ++j)
        {
            //Create f_ij
            MPVariable* const flow = flows.at(std::to_string(i)+std::to_string(j));
            //Calculate euclidean distance between clusters
            const double euclDist = distances.at(std::to_string(i)+std::to_string(j));

            objective->SetCoefficient(flow, euclDist);
        }
    }
    // Minimize Earth Mover's Distance
    objective->SetMinimization();

    const MPSolver::ResultStatus result_status = solver->Solve();
    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL) {
        throw runtime_error("The problem does not have an optimal solution!\n");
    }

    double retVal = objective->Value();
    delete solver;
    return retVal;
}

double euclideanDist(tuple<int, int> firstCord, tuple<int, int> secondCord)
{
    int x1 = get<0>(firstCord);
    int x2 = get<0>(secondCord);

    int y1 = get<1>(firstCord);
    int y2 = get<1>(secondCord);

    int firstTerm = pow(x1-x2, 2);
    int secondTerm = pow(y1-y2, 2);

    return sqrt(firstTerm + secondTerm);
}