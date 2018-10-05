
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <Graph.h>
#include <Network.h>


using namespace std;


int main(int argc, char* argv[]){

    cout.precision(6);

    //const char* file = "..\\as-skitter.txt";
    const char* file = "..\\oregon1_010331.txt";
    if(argc > 1)
        file = argv[1];

    cout << "reading file " << file << endl;

    auto network = Network(Graph::fromTxt(file), &Network::bestResponseSumDist);
    auto a = 1.0;
    if(argc > 2)
        a = atof(argv[2]);
    cout << "using alpha = " << a << endl;
    network.m_alpha = a;

    cout << "n: " << network.graph().n() << endl;
    cout << "m: " << network.graph().m() / 2 << " (undirected)" << endl;
    cout << "done reading file" << endl;
    cout << endl;

    auto m = network.graph().m();
    do {
        auto stats = network.stats();
        cout << "After " << network.graph().m() - m << " built edges ..." << endl;
        cout << "approx ratio: " << stats.approx_ratio << endl;
        cout << "avg a. ratio: " << stats.avg_approx << endl;
        cout << "stable ratio: " << stats.stable_ratio << endl;
        cout << endl;
        cout << "performing round " << network.m_round + 1 << endl;
    } while (!network.performRound(false));
    cout << "network converged in " << network.m_round << " round(s)" << endl;

    return 0;
}