
#include <iostream>
#include <fstream>
#include <cassert>

#include <Graph.h>
#include <Network.h>

using namespace std;


int main(int argc, char* argv[]){


    auto network = Network(Graph::createCircle(1000), &Network::bestResponseSumDist);
    network.m_alpha = 1.0;
    network.m_c = -1;

    while(!network.performRound(true))
        cout << "finished round " << network.m_round << endl;
    cout << "network converged" << endl;

    // ofstream file("test.dot");
    // file << network.graph();

    return 0;
}