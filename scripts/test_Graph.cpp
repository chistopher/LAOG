
#include <iostream>
#include <algorithm>
#include <numeric>

#include <Graph.h>


using namespace std;

// change to test other starting graphs
const auto seed = 1337;


void assert(bool cond){
    if(!cond){
        cout << "assertion failed" << endl;
        exit(1);
    }
}



void testMaxLayer(){
    auto g = Graph::createRandomTree(100, seed);
    const auto v = 23;

    auto dist = g.distances(v);
    auto distTo5 = g.distances(v, 5);
    for(unsigned int i=0; i<g.n(); ++i)
        if(dist[i] > 5)
            assert(distTo5[i] == -1);
        else
            assert(distTo5[i] == dist[i]);
}

void testDistWithEdge(){
    auto g = Graph::createRandomTree(100, seed);
    const auto v = 23;
    const auto u = 67;

    // assert v and u not connected (CAUTION! CAN RANDOMLY FAIL)
    const auto & vNeighbors = g.neighbors(v);
    assert(find(vNeighbors.begin(), vNeighbors.end(), u) == vNeighbors.end());

    auto dist = g.distances(v);
    auto improvement = g.distImprovementOfEdge(v, u, 10000, dist);
    auto dist2 = g.distances(v);

    // assert constness of g.distImprovement
    for(unsigned int i=0; i<g.n(); ++i)
        assert(dist[i] == dist2[i]);

    // check correctness
    auto oldDistSum = accumulate(dist.begin(), dist.end(), 0);
    g.connect(v, u);
    auto newDist = g.distances(v);
    auto newDistSum = accumulate(newDist.begin(), newDist.end(), 0);
    assert(newDistSum == oldDistSum - improvement);
}

void testDistImprovementOfTwoNeighs(){
    auto g = Graph::createRandomTree(100, 1337);
    const auto v = 1;

    auto dist = g.distances(v);
    auto distImp = g.distImprovementOfTwoNeighs(v);
    for(unsigned int i=0; i<g.n(); ++i){
        if(dist[i] != 2)
            assert(distImp[i] == 0);
        else {
            auto imp1 = g.distImprovementOfEdge(v, i, g.n(), dist);
            auto imp2 = distImp[i];
            assert(imp1 == imp2); // assert correct dist improvement
        }
    }
}


int main(int argc, char* argv[]){

    testMaxLayer();
    testDistWithEdge();
    testDistImprovementOfTwoNeighs();

    cout << "all tests passed" << endl;

    return 0;
}
