#include "Lab6.h"
#include <stack>

// [[Rcpp::plugins(cpp11)]]

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
    // cout <<gR;

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
    // cout<<g;
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
    // cout<<g1;
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

//[[Rcpp::export]]
void testSCC(string s) {

    Graph g1(s);
    vector<size_t> strongConnectedComponets = find_strongly_connected_components(g1);
} 

//[[Rcpp::export]]
void testCC(string s) {

    Graph g1(s);
    vector<size_t> connectedComponets = find_connected_components(g1);
} 

//[[Rcpp::export]]
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


//---------------------------------------------------------------

/*** R
require("igraph")
library(ggplot2)
library(gridExtra)
library(grid)



#Plot the data read from graph file.
graph.plot <- function(links, direct=T){
    net <- graph_from_data_frame( d=links, directed=direct )
    plot(net, vertex.size=30, vertex.label.cex=2)
    
    return (NULL)
}

#Read the graph file, return the inside data. An option to plot or not plot by specifying plot parameter.
graph.from.file <- function(input.file, plot=TRUE, direct=T){
    ##
    links <- read.table(input.file, header = FALSE, sep = '\t', quote = "", stringsAsFactors = FALSE)
    colnames(links) <- c("from", "to")
    ##
    if(plot){
        graph.plot(links, direct)
    }
    
    return (links)
}

#Compare the two graph from two files. For small examples, use plot=TRUE; for the big one, use plot=FALSE.
graph.compare <- function(file1, file2, plot=TRUE){
    g1 <- graph.from.file(file1, plot=FALSE)
    g2 <- graph.from.file(file2, plot=FALSE)
    
    g1 <- g1[order(g1[,1], g1[,2]),]
    g2 <- g2[order(g2[,1], g2[,2]),]
    
    #  print(g1)
    # print(g2)
    
    test <- TRUE
    test <- test & (nrow(g1) == nrow(g2)) & (ncol(g1) == ncol(g2))
    test <- test & (sum(g1 == g2) == nrow(g1) * ncol(g1))
    
    if(!test){
        stop("Sorry, not equal!")
    }else{
        message("Congratulations!")
        if(plot){
            graph.plot(g1)
            graph.plot(g2)
        }
    }
    
    return (test)
}

#Randomly generate big graph file.
random.graph <- function(number.of.nodes = 100000, number.of.edges = 1000000, output.file = "in_test_big.txt"){
    nodes <- paste("N", seq(from=1, to=number.of.nodes), sep="")
    
    parent.nodes <- c(nodes, nodes[sample(c(1:number.of.nodes), number.of.edges-number.of.nodes, TRUE)])
    parent.nodes <- parent.nodes[sample(c(1:length(parent.nodes)), length(parent.nodes), FALSE)]
    
    child.nodes <- c(nodes, nodes[sample(c(1:number.of.nodes), number.of.edges-number.of.nodes, TRUE)])
    child.nodes <- child.nodes[sample(c(1:length(child.nodes)), length(child.nodes), FALSE)]
    
    links <- data.frame(from = parent.nodes, to = child.nodes)
    write.table(links, output.file, append = FALSE, sep = '\t', quote = FALSE, row.names = FALSE, col.names = FALSE)
    message("Scrpt is Done")
}

if ( !testall() ) {
    exit (1);
}

random.graph(1250,  20000, "test_node_1.txt")
random.graph(2500,  20000, "test_node_2.txt")
random.graph(5000,  20000, "test_node_3.txt")
random.graph(10000, 20000, "test_node_4.txt")
random.graph(20000, 20000, "test_node_5.txt")

numberNodesSCC <- c(1250,2500,5000,10000, 20000 )


runtimeSCCnode <- 1:5
fileNames <- c("test_node_1.txt", "test_node_2.txt", "test_node_3.txt", "test_node_4.txt", "test_node_5.txt")

for ( i in 1:5 ) {
    
    
    runtimeSCCnode[i] <- system.time(testSCC(fileNames[i]))[["user.self"]]
    
}

runtimeCCnode <- 1:5

random.graph(12500,  100000, "test_node_1_cc.txt")
random.graph(25000,  100000, "test_node_2_cc.txt")
random.graph(50000,  100000, "test_node_3_cc.txt")
random.graph(70000,  100000, "test_node_4_cc.txt")
random.graph(100000, 100000, "test_node_5_cc.txt")

numberNodesCC <- c(12500,25000,50000,70000, 100000 )

fileNames <- c("test_node_1_cc.txt", "test_node_2_cc.txt", "test_node_3_cc.txt", "test_node_4_cc.txt", "test_node_5_cc.txt")

for ( i in 1:5 ) {
    
    
    runtimeCCnode[i] <- system.time(testCC(fileNames[i]))[["user.self"]]
    
}



random.graph(10000,  12500,"test_edge_1.txt")
random.graph(10000,  25000, "test_edge_2.txt")
random.graph(10000,  50000, "test_edge_3.txt")
random.graph(10000,  100000, "test_edge_4.txt")

numberOfEdgesCC <- c(12500,25000,50000,100000)
numberOfEdgesSCC <- c(12500,25000,50000,100000)

runtimeCCedge <- 1:4
runtimeSCCedge <- 1:4
fileNames <- c("test_edge_1.txt", "test_edge_2.txt", "test_edge_3.txt", "test_edge_4.txt")

for ( i in 1:4 ) {

    runtimeCCedge[i] <- system.time(testCC(fileNames[i]))[["user.self"]]
    runtimeSCCedge[i] <- system.time(testSCC(fileNames[i]))[["user.self"]]
    
}

graph.from.file("test_cc_0.txt", plot=TRUE, direct=T) 
graph.from.file("test_cc_1.txt", plot=TRUE, direct=T) 
graph.from.file("test_cc_2.txt", plot=TRUE, direct=T) 
graph.from.file("test_cc_3.txt", plot=TRUE, direct=T) 
graph.from.file("test_cc_4.txt", plot=TRUE, direct=T) 


graph.from.file("test_scc_0.txt", plot=TRUE, direct=T) 
graph.from.file("test_scc_1.txt", plot=TRUE, direct=T) 
graph.from.file("test_scc_2.txt", plot=TRUE, direct=T) 
graph.from.file("test_scc_3.txt", plot=TRUE, direct=T) 
graph.from.file("test_scc_4.txt", plot=TRUE, direct=T) 



#plot(numberNodes, runtimeRnode, type="b", xlab="number of nodes", ylab="runtime (second)")





dataCC <- data.frame(
    runtime = c(runtimeCCnode),
    graphType = c(rep(c("Connected Components"), each=5)), 
    x = c(numberNodesCC)
    
)

dataSCC <- data.frame(
    runtime = c(runtimeSCCnode),
    graphType = c(c(rep(c("Strongly Connected Components"), each=5))),
    x = c(numberNodesSCC)
    
)


pCC <- ggplot(dataCC, aes(x,runtime,group=graphType)) +
    geom_point(color='red',aes( shape = graphType)) +
    geom_line( color='red',aes( linetype = graphType) ) 
pCC <- pCC + labs(x="Number of Nodes", y="Runtime(seconds)")
pCC <- pCC + ggtitle("Finding Connected Components Function on Number of Nodes")


grid.arrange(pCC, nrow=1)

pSCC <- ggplot(dataSCC, aes(x,runtime,group=graphType)) +
    geom_point(color='blue', aes( shape = graphType)) +
    geom_line( color='blue', aes( linetype = graphType) ) 
pSCC <- pSCC + labs(x="Number of Nodes", y="Runtime(seconds)")
pSCC <- pSCC + ggtitle("Finding Strongly Connected Components Function on Number of Nodes")


grid.arrange(pSCC, nrow=1)


dataCC <- data.frame(
    runtime = c(runtimeCCedge),
    graphType = c(rep(c("Connected Components"), each=4)),
    x = c(numberOfEdgesCC)
    
)

dataSCC <- data.frame(
    runtime = c(runtimeSCCedge),
    graphType = c(rep(c("Strongly Connected Components"), each=4)),
    x = c(numberOfEdgesSCC)
    
)

pCC <- ggplot(dataCC, aes(x,runtime,group=graphType)) +
    geom_point(color='red', aes(shape = graphType)) +
    geom_line( color='red', aes( linetype = graphType)) 
pCC <- pCC + labs(x="Number of Edges", y="Runtime(seconds)")
pCC <- pCC + ggtitle("Finding Connected Components Function on Number of Edges")


pSCC <- ggplot(dataSCC, aes(x,runtime,group=graphType)) +
    geom_point(color='blue', aes( shape = graphType)) +
    geom_line( color='blue', aes( linetype = graphType)) 
pSCC <- pSCC + labs(x="Number of Edges", y="Runtime(seconds)")
pSCC <- pSCC + ggtitle("Finding Strongly Connected Components Function on Number of Edges")

grid.arrange(pCC, nrow=1)

grid.arrange(pSCC, nrow=1)

*/