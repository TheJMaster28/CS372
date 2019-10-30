#include "Lab7.h"

double distance ( Node &a, Graph &G ) {
    return G.NodeGetDistance(a);
}

void distance ( Node &a, Graph &G, double dis ) {
    G.NodeSetDistance(a,dis);
}

void BFS ( Graph &G, Node &s) {
    // Input : graph G, which can be either undirected or directed
    // Output : the effect of BFS is that  all nodes
    // 1. are marked with a distance label
    // perform BFS on the graph from a gi ven node s

    // for now, nodes are already have infinty distances
    // might change later
    G.Reset();

    queue<Node> Q;

    distance(s,G,0);

    Q.push(s);

    while (!Q.empty()) {

        Node u = Q.front();
        Q.pop();
        for ( Node v : G.getAdjNodes(u) ) {
            if ( distance(v,G) == numeric_limits<double>::infinity()) {
                Q.push(v);
                distance(v,G,distance(u,G)+1);
            }
        }
        
    }

}



bool testall() {
    bool passed = true;

    Graph g1("test_0.txt");
    Node s = g1.getNode(0);
    
    BFS(g1,s);
    
    cout<<g1;

    return passed;

}

int main () {

    if ( !testall() ) {
        cout<<"Tests failed :("<<endl;
    }
    
}