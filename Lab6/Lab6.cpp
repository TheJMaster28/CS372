#include "Lab6.h"
#include <stack>

int NodeClock;

void exploreIt ( Graph &G, Node v) {
    // create stack 
    stack<Node> DiscoveredNodes;
    // create list
    list< Node > aAdjList;
    
    // push source node into stack
    DiscoveredNodes.push(v);
    
    while ( !DiscoveredNodes.empty() ) {
        // set previst and visted if top is not visted
        if ( !G.NodeGetVist(DiscoveredNodes.top())) {
            G.NodeSetPre(DiscoveredNodes.top(),++NodeClock);
            G.NodeSetVisted(DiscoveredNodes.top(),true);
        }
        
        // get adjacent nodes of top
        aAdjList = G.getAdjNodes(DiscoveredNodes.top());
        
        // push not visted nodes in stack
        for ( Node b: aAdjList ) {
            if ( !G.NodeGetVist(b) ) {
                DiscoveredNodes.push(b);
            }
            
        }

        // if top has been visted
        if ( G.NodeGetVist(DiscoveredNodes.top()) ) {
            // set Post vist if it has not been set before
            if ( G.NodeGetPost(DiscoveredNodes.top()) == 0 ) 
                G.NodeSetPost(DiscoveredNodes.top(), ++NodeClock);
            // pop off of stack to explore other nodes
            DiscoveredNodes.pop();
        }
    }
}

void DFS_iterative(Graph &G) {
    NodeClock = 0;
    // set all nodes visted to false
     for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        G.NodeSetVisted(a,false);
    }
    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            exploreIt(G, a);
        }
    }
}


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


Graph reverse ( Graph &G) {
    Graph gR(G.getFileName());

    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        // gR.addNode(G.getNode(i));
        gR.clearAdjNodes(G.getNode(i));
    }

    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node v = G.getNode(i);
        list<Node> vAdjNodes = G.getAdjNodes(v);
        for ( Node s : vAdjNodes ) {
            gR.addEdge(s,v);
            
        } 
    }
    return gR;
}

vector < size_t > find_strongly_connected_components ( Graph & G) {
    
    Graph gR = reverse(G);
    DFS_recursive(gR);
    cout <<gR;

    vector < Node > orderPost;

    Node max = gR.getNode(0);
    vector < Node > nodesList;
    nodesList.resize(gR.num_nodes());

    for ( size_t i = 0; i < gR.num_nodes(); i++ ) {
        nodesList.push_back(gR.getNode(i));
    }
    
    for ( size_t i = 0; i < gR.num_nodes(); i++ ) { 
        int place = 0;
        for ( size_t k = 0; k < nodesList.size(); k++ ) { 
            Node a = nodesList[k];
            if ( gR.NodeGetPost(max) < gR.NodeGetPost(a)) {
                max = a;
                place = k;
            }
        }
        nodesList.erase(nodesList.begin() + place);
        orderPost.push_back(max);
    }



    
     

    vector < size_t > ccNum;
    // set globel node clock to track pre and post times
    NodeClock = 0;
    // set all nodes to false
    for ( size_t i = 0; i < orderPost.size(); i++ ) {
        Node a = G.getNode(i);
        G.NodeSetVisted(a,false);
    }

    for ( size_t i = 0; i < orderPost.size(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            ccNum.push_back(a.id());
            explore(G, a);
        }
    }

    return ccNum;

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

    Graph g1("test_scc_0.txt");

    cout<<g1;

    vector<size_t> strongConnectedComponets = find_strongly_connected_components(g1);

    printVector(strongConnectedComponets);

    Graph g2("test_scc_1.txt");

    strongConnectedComponets = find_strongly_connected_components(g2);

    printVector(strongConnectedComponets);

    return passed;
}

int main () {
    bool passed = testall();

    if ( !passed ) {
        cout<<"Tests have failed :("<<endl;
    }

}