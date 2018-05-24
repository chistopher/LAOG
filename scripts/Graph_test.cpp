
#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include <Graph.h>
#include <Network.h>
#include <numeric>

using namespace std;


int main(int argc, char* argv[]){

    auto g = Graph::createRandomTree(100);
    const int v = 13;

    auto realDist = g.distances(v);
    auto sumDist = accumulate(realDist.begin(), realDist.end(), 0);

    // test maxLayer
    auto distTo5 = g.distances(v, 5);
    for(unsigned int i=0; i<g.n(); ++i)
    {
        if(realDist[i] > 5)
            assert(distTo5[i] == -1);
        else
            assert(distTo5[i] == realDist[i]);
    }

    // test distWidthEdge
    auto start1 = chrono::high_resolution_clock::now();
    auto improvement = g.distImprovementOfEdge(v, 67, 10000, realDist);
    auto dists2 = g.distances(v);
    auto end1 = chrono::high_resolution_clock::now();
    assert(improvement > 0); // secures that v was not connected to 67

    for(unsigned int i=0; i<g.n(); ++i) // check that nothing has changed
        assert(dists2[i] == realDist[i]);

    auto start2 = chrono::high_resolution_clock::now();
    g.connect(v, 67);
    auto newDists = g.distances(v);
    auto end2 = chrono::high_resolution_clock::now();

    auto sumDistWithEdge = accumulate(newDists.begin(), newDists.end(), 0);
    assert(sumDistWithEdge == sumDist - improvement);

    cout << (end1 - start1).count() << endl;
    cout << (end2 - start2).count() << endl;
    return 0;
}