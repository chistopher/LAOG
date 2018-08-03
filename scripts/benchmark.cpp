
#include <iostream>
#include <string>
#include <chrono>
#include <utility>

#include <Graph.h>
#include <Network.h>

using namespace std;

// benchmark configuration
const auto repetitions = 50;
using accuracy = std::chrono::milliseconds;

vector<int> nodeRange = {200, 400, 600, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10'000};

// command line choices
using BestResponseFunction = int(Network::*)(int) const;
BestResponseFunction functions[] = {
        &Network::neighNaive,
        &Network::neighGood,
        &Network::distNaive,
        &Network::distImproOfEdge,
        &Network::distImproOfEdgeMaxLayer,
        &Network::distWithSets,
        &Network::distDAGandBFS,
        &Network::distDAGandBFSStatic,
};
string funcNames[] = {
        "neighNaive",
        "neighGood",
        "distNaive",
        "distImproOfEdge",
        "distImproOfEdgeMaxLayer",
        "distWithSets",
        "distDAGandBFS",
        "distDAGandBFSStatic",
};

// global state
auto alpha = 0.4;
auto usedBR = functions[0];

auto checksum = 0;


auto measure_single(int n, int run){
    auto network = Network(Graph::createCircle(n), usedBR, &Network::linearCost);
    network.seed(n+run);
    network.m_alpha = alpha;
    network.m_c = 0.0;
    network.m_greedy = false;

    auto start = std::chrono::high_resolution_clock::now();
    while(true)
        if(network.performRound(true))
            break;
    auto end = std::chrono::high_resolution_clock::now();

    checksum += network.m_round;
    cout << n << "/" << run << "\trounds: " << network.m_round << "      ";
    cout.flush();
    cout << '\r';

    return std::chrono::duration_cast<accuracy>(end-start).count();
}

auto measure_avg(int n){
    auto avg = 0;
    for(auto i=0;i<repetitions;++i){
        avg += measure_single(n, i);
    }
    return avg / repetitions;
}

int main(int argc, char* argv[]){

    // cli stuff
    int numFuncs = sizeof(functions) / sizeof(BestResponseFunction);
    if(argc < 2 || atoi(argv[1]) >= numFuncs){
        cout << "first arg no valid bestResponse function\n";
        for(auto i=0; i<numFuncs; ++i)
            cout << '\t' << i << ' ' << funcNames[i] << '\n';
        return 0;
    }

    // set global state
    auto funcNum = atoi(argv[1]);
    alpha = funcNum < 2 ? 0.4 : 1.0;
    usedBR = functions[funcNum];

    // log stuff
    cout << "using\t" << funcNames[funcNum] << '\n'
         << "alpha\t" << alpha << '\n'
         << "reps\t" << repetitions << '\n'
         << "n\t";
    for(auto n : nodeRange)
        cout << n << (n!=nodeRange.back() ? ' ' : '\n');

    // benchmark
    for(auto n : nodeRange){
        cout << measure_avg(n) << "                       " << endl;

        if(argc > 2){
            clog << n << '\t' << checksum << endl;
            checksum = 0;
        }
    }

    return 0;
}
