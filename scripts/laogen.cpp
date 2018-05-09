
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <map>

#include <Graph.h>
#include <Network.h>

using namespace std;


map<string, string> parseArgs(int argc, char** argv){
    map<string, string> params;
    params["starting"] = "circle";
    params["n"] = "1000";
    for (int i=1; i<argc; i++) {
        // Get current and next argument
        std::string arg  = argv[i];
        std::string next = (i+1 < argc ? argv[i+1] : "");

        params[std::move(arg)] = std::move(next);
        i++;
    }
    return params;
}


int main(int argc, char* argv[]){
    if(argc < 2){
        clog << "usage: laogen [n 4]  [dist dist|two] [cost linear|poly a 0.4 c 0.0] [starting circle|tree|path] [greedy 0|1] [gexf 0|1] [path <>]";
    }

    auto params = parseArgs(argc, argv);

    // choose starting graph
    auto starting = params["starting"];
    auto graph = (starting == "path") ? Graph::createPath(stoi(params["n"])) :
                 ((starting == "tree") ? Graph::createRandomTree(stoi(params["n"])) :
                 Graph::createCircle(stoi(params["n"])));

    // choose dist metric
    auto dist = &Network::bestResponseTwoNeigh;
    if(params["dist"] == "dist")
        dist = &Network::bestResponseSumDist;

    // choose edge cost metric
    auto cost = &Network::linearCost;
    if(params["cost"] == "poly")
        cost = &Network::polyCost;

    auto greedy = false;
    if(params["greedy"] == "1")
        greedy = true;

    auto a = 0.4;
    if(params["a"] != "")
        a = stod(params["a"]);

    auto c = 0.4;
    if(params["c"] != "")
        a = stod(params["c"]);

    string path = ".";
    if(params["path"] != "") {
        path = params["path"];
        if(path.back() == '/' && path.length() > 1)
            path.pop_back();
    }

    auto save_intermediate = params["gexf"] == "1";


    auto network = Network(move(graph), dist, cost, starting);
    network.m_alpha = a;
    network.m_c = c;
    network.m_greedy = greedy;

    do {
        if(save_intermediate)
            network.save_gexf(path);
        cout << "starting round " << network.m_round + 1 << endl;
    } while (!network.performRound(true));
    cout << "network converged" << endl;

    if(!save_intermediate)
        network.save_dot(path);

    return 0;
}