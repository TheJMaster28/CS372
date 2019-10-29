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


void explore ( Graph &G, Node &v, vector<size_t> &cc, size_t ccNum) {
    // set node to visted and previst time
    G.NodeSetVisted(v,true);
    G.NodeSetPre(v,++NodeClock);
    cc[v.id()] = ccNum;
    // get adjacent nodes
    list<Node> vAdjNodes = G.getAdjNodes(v);
    for ( Node i : vAdjNodes ) {
        // recursively go explore unvisted nodes
        if ( !G.NodeGetVist(i) ) {
            explore(G, i, cc, ccNum);
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

    vector < size_t > cc;
    cc.resize(G.num_nodes());
    size_t ccNum= 0;
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
            explore(G, a,cc, ccNum++);
        }
    }

    return cc;


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

    Node max;
    vector < Node > nodesList;
    nodesList.resize(gR.num_nodes());

    for ( size_t i = 0; i < gR.num_nodes(); i++ ) {
        nodesList[i] = gR.getNode(i);
    }
    
    for ( size_t i = 0; i < gR.num_nodes(); i++ ) { 
        max = nodesList[0];
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


    
     

    vector < size_t > cc;
    cc.resize(G.num_nodes());
    size_t ccNum = 0;
    // set globel node clock to track pre and post times
    NodeClock = 0;
    // set all nodes to false
    for ( size_t i = 0; i < orderPost.size(); i++ ) {
        Node a = G.getNode(orderPost[i].id());
        G.NodeSetVisted(a,false);
    }

    for ( size_t i = 0; i < orderPost.size(); i++ ) {
        Node a = G.getNode(orderPost[i].id());
        if ( !a.getVisted() ) {
            explore(G, a, cc, ccNum++);
        }
    }

    return cc;

}

void printVector( vector<size_t>  &a) {
    for ( size_t i = 0; i < a.size(); i++) {
        cout<<i<<":"<<a[i]<<' ';
    }
    cout << endl;
}


bool testCC(string s, vector<size_t> answer) {
    cout<<"Testing Connected Component "<<s<<endl;
    Graph g(s);
    cout<<g;
    vector<size_t> connectedComponets = find_connected_components(g);

    cout<<"\nAnswer:"<<endl;
    printVector(connectedComponets);


    cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
    if ( answer.size() != connectedComponets.size() ) { return false; }
    for (size_t i = 0; i < answer.size(); i++) {
        if ( connectedComponets[i] != answer[i] ) {
            return false;
        }
    }
    return true;
}

bool testSCC(string s, vector<size_t> answer) {

    cout<<"Testing Strongly Connected Component "<<s<<endl;
    Graph g1(s);
    cout<<g1;
    vector<size_t> strongConnectedComponets = find_strongly_connected_components(g1);
    cout<<"\nAnswer:"<<endl;
    printVector(strongConnectedComponets);

    cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
    if ( answer.size() != strongConnectedComponets.size() ) { return false; }
    for (size_t i = 0; i < answer.size(); i++) {
        if ( strongConnectedComponets[i] != answer[i] ) {
            return false;
        }
    }
    return true;
}


bool testall() {

    // Connected Components
    if ( !testCC("test_cc_0.txt",{0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,2}) ) {
        cout<<"Coneccted Components Test 1 Failed"<<endl;
        return false;
    }

    if ( !testCC("test_cc_1.txt",{0,0,0,0,1,1,1}) ) {
        cout<<"Coneccted Components Test 2 Failed"<<endl;
        return false;
    }


    if ( !testCC("test_cc_2.txt",{0,0,0,0,0,0}) ) {
        cout<<"Coneccted Components Test 3 Failed"<<endl;
        return false;
    }

    if ( !testCC("test_cc_3.txt",{0,0,0,0,0,0,1,1,1,2,2}) ) {
        cout<<"Coneccted Components Test 4 Failed"<<endl;
        return false;
    }

    if ( !testCC("test_cc_4.txt",{0,0,0,0,0,0}) ) {
        cout<<"Coneccted Components Test 5 Failed"<<endl;
        return false;
    }

    // Strongly Connected Components
    if ( !testSCC("test_scc_0.txt",{2,2,2,3,4,4,0,0,1}) ) {
        cout<<"Strongly Connected Components Test 1 Failed"<<endl;
        return false;
    }
    
    if ( !testSCC("test_scc_1.txt",{2,2,1,0,2,0,0,0,0}) ) {
        cout<<"Strongly Connected Components Test 2 Failed"<<endl;
        return false;
    }

    if ( !testSCC("test_scc_2.txt",{13,10,9,8,7,5,16,15,2,11,14,4,12,1,6,3,0}) ) {
        cout<<"Strongly Connected Components Test 3 Failed"<<endl;
        return false;
    }

    if ( !testSCC("test_scc_3.txt",{1,0,2,5,4,3}) ) {
        cout<<"Strongly Connected Components Test 4 Failed"<<endl;
        return false;
    }
    
    if ( !testSCC("test_scc_4.txt",{3,3,3,2,2,2,4,1,0}) ) {
        cout<<"Strongly Connected Components Test 5 Failed"<<endl;
        return false;
    }

    return true;
}

int main () {
    bool passed = testall();

    if ( !passed ) {
        cout<<"Tests have failed :("<<endl;
    }

    else {
        cout << "ALL TESTS HAVE PASSED!"<<endl;
    }

}