
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <Graph.h>
#include <Network.h>

using namespace std;


int main(int argc, char* argv[]){

    auto network = Network(Graph::createRandomTree(3000, 1337), &Network::bestResponseSumDist, &Network::linearCost);
    network.m_alpha = 1.0;
    network.m_c = 0.0;
    
    network.m_greedy = true;

    while(!network.performRound(true))
        cout << "finished round " << network.m_round << endl;

    cout << "network converged" << endl;

    return 0;
}