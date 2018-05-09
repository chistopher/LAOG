
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <map>
#include <cstring>

#include <Graph.h>
#include <Network.h>

using namespace std;


map<string, string> parseArgs(int argc, char** argv){
    map<string, string> params;
    params["start"] = "circle";
    params["n"] = "1000";
    for (int i=1; i<argc; i++) {
        // Get current and next argument
        if(argv[i][0] != '-')
            continue;
        std::string arg  = argv[i]+1; // +1 to skip the -
        std::string next = (i+1 < argc ? argv[i+1] : "");

        params[std::move(arg)] = std::move(next);
        ++i;
    }
    return params;
}


int main(int argc, char* argv[]){
    if(argc < 2 || 0 == strcmp(argv[1], "--help") || 0 == strcmp(argv[1], "-help")){
        clog << "usage: ./laogen [-n anInt]\t\t  // number of nodes\n"
             << "\t\t[-dist dist|two]          // distance cost function\n"
             << "\t\t[-cost linear|poly]       // edge cost function\n"
             << "\t\t[-a aFloat]               // factor or exponent for degree in edge cost\n"
             << "\t\t[-c aFloat]               // constant offset for edge cost\n"
             << "\t\t[-start circle|tree|path] // starting graph configuration\n"
             << "\t\t[-greedy 0|1]             // best response or greedy\n"
             << "\t\t[-gexf 0|1]               // save in gexf format (also intermediate saves)\n"
             << "\t\t[-path aPath]             // path where the graph(s) should be saved\n";
        return 0;
    }

    auto params = parseArgs(argc, argv);

    // choose starting graph
    auto starting = params["start"];
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

    auto c = 0.0;
    if(params["c"] != "")
        c = stod(params["c"]);

    string path = ".";
    if(params["path"] != "") {
        path = params["path"];
        if(path.back() == '/' && path.length() > 1)
            path.pop_back();
    }

    auto save_intermediate = params["gexf"] == "1";

    // DO STUFF
    auto network = Network(move(graph), dist, cost, starting);
    network.m_alpha = a;
    network.m_c = c;
    network.m_greedy = greedy;

    do {
        if(save_intermediate)
            network.save_gexf(path);
        clog << "starting round " << network.m_round + 1 << endl;
    } while (!network.performRound(true));
    clog << "network converged in " << network.m_round - 1 << " round(s)" << endl;

    if(!save_intermediate)
        network.save_dot(path);

    return 0;
}