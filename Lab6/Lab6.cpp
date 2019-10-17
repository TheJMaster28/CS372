#include "Lab6.h"
#include <stack>

int NodeClock;

void explore ( Graph &G, Node &v) {
    // set node to visted and previst time
    G.NodeSetVisted(v,true);
    G.NodeSetPre(v,++NodeClock);
    // get adjacent nodes
    list<Node> vAdjNodes = G.getAdjNodes(v);
    for ( Node i : vAdjNodes ) {
        // recursively go explore unvisted nodes
        if ( !G.NodeGetVist(i) ) {
            explore(G, i);
        }

    }
    // set post
    G.NodeSetPost(v,++NodeClock);
}

void DFS_recursive(Graph &G) {
    // set globel node clock to track pre and post times
    NodeClock = 0;
    // set all nodes to false
    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        G.NodeSetVisted(a,false);
    }

    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            explore(G, a);
        }
    }
}

vector < size_t > find_connected_components ( Graph & G ) {

    vector < size_t > ccNum;
    // set globel node clock to track pre and post times
    NodeClock = 0;
    // set all nodes to false
    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        G.NodeSetVisted(a,false);
    }

    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            ccNum.push_back(a.id());
            explore(G, a);
        }
    }

    return ccNum;


}


vector < size_t > find_strongly_connected_components ( Graph & G) {

}

void printVector( vector<size_t>  &a) {
    for ( size_t i = 0; i < a.size(); i++) {
        cout<<a[i]<<' ';
    }
    cout << endl;
}

bool testall() {
    bool passed = true;
    Graph g("test.txt");
    cout<<g;
    vector<size_t> connectedComponets = find_connected_components(g);

    printVector(connectedComponets);


    return passed;
}

int main () {
    bool passed = testall();

    if ( !passed ) {
        cout<<"Tests have failed :("<<endl;
    }

}