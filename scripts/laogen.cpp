
#include <iostream>
#include <map>
#include <string>
#include <cstring>

#include <Graph.h>
#include <Network.h>

using namespace std;


map<string, string> parseArgs(int argc, char** argv){
    map<string, string> params;
    for (int i=1; i<argc; i++) {
        // Get current and next argument
        if(argv[i][0] != '-')
            continue;
        std::string arg  = argv[i]+1; // +1 to skip the -
        std::string next = (i+1 < argc ? argv[i+1] : "");

        // if next is empty or a new param means current is treated as bool flag
        // a new param will start with '-' and end with no number
        if(next.length() == 0 || (next.front() == '-' && !('0' <= next.back() && next.back() <= '9'))){
            params[std::move(arg)] = "1";
        } else {
            // advance one additional position if next is used
            params[std::move(arg)] = std::move(next);
            ++i;
        }
    }
    return params;
}


int main(int argc, char* argv[]){

    if(argc < 2 || 0 == strcmp(argv[1], "--help") || 0 == strcmp(argv[1], "-help")){
        clog << "usage: ./laogen [-n anInt]\t\t  // number of nodes               default 1000\n"
             << "\t\t[-dist dist|two]          // distance cost function        default two\n"
             << "\t\t[-cost linear|poly]       // edge cost function            default linear\n"
             << "\t\t[-a aFloat]               // factor or exponent for cost   default 0.4\n"
             << "\t\t[-c aFloat]               // constant offset for edge cost default 0.0\n"
             << "\t\t[-start circle|tree|path] // starting graph configuration  default circle\n"
             << "\t\t[-greedy 0|1]             // best response or greedy       default 0\n"
             << "\t\t[-random 0|1]             // enables random scheduling     default 1\n"
             << "\t\t[-seed anInt]             // seed for scheduling and tree  default 1337\n"
             << "\t\t[-gexf 0|1]               // save intermediates as gexf    default 0\n"
             << "\t\t[-file aString]           // file name for output graph    default a good one\n";
        return 0;
    }

    // read params
    auto params = parseArgs(argc, argv);
    auto n = !params["n"].empty() ? stoi(params["n"]) : 1000;
    auto seed = !params["seed"].empty() ? stoi(params["seed"]) : 1337;
    auto start = !params["start"].empty() ? params["start"] : "circle";
    auto graph = (start == "path") ? Graph::createPath(n) :
                 ((start == "tree") ? Graph::createRandomTree(n, seed) :
                 Graph::createCircle(n));
    auto dist = params["dist"] == "dist" ? &Network::bestResponseSumDist : &Network::bestResponseTwoNeigh;
    auto cost = params["cost"] == "poly" ? &Network::polyCost : &Network::linearCost;
    auto a = !params["a"].empty() ? stod(params["a"]) : 0.4;
    auto c = !params["c"].empty() ? stod(params["c"]) : 0.0;
    auto greedy = params["greedy"] == "1";
    auto random = params["random"] != "0";
    auto save_intermediate = params["gexf"] == "1";
    auto file = params["file"];

    // DO STUFF
    auto network = Network(move(graph), dist, cost, start);
    network.m_alpha = a;
    network.m_c = c;
    network.m_greedy = greedy;
    network.seed(seed);

    do {
        if(save_intermediate)
            network.save_gexf(file);
        clog << "starting round " << network.m_round + 1 << endl;
    } while (!network.performRound(random));
    clog << "network converged in " << network.m_round << " round(s)" << endl;

    if(!save_intermediate)
        network.save_dot(file);

    return 0;
}
