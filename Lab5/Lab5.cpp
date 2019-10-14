#include "Lab5.h"
#include <stack>
// [[Rcpp::plugins(cpp11)]]


int NodeClock = 0;

void explore ( Graph &G, Node &v) {
    G.NodeSetVisted(v,true);
    G.NodeSetPre(v,++NodeClock);
    list<Node> vAdjNodes = G.getAdjNodes(v);
    for ( Node i : vAdjNodes ) {
        Node a = G.getNode(i.id());
        if ( !a.getVisted() ) {
            explore(G, a);
        }

    }
    G.NodeSetPost(v,++NodeClock);
}

void DFS_recursive(Graph &G) {
    NodeClock = 0;
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

void exploreIt ( Graph &G, Node v) {
    stack<Node> DiscoveredNodes;
    list< Node > aAdjList;
    
    DiscoveredNodes.push(v);
    
    while ( !DiscoveredNodes.empty() ) {

        aAdjList = G.getAdjNodes(DiscoveredNodes.top());

        if ( !G.NodeGetVist(DiscoveredNodes.top())) {
            G.NodeSetPre(DiscoveredNodes.top(),++NodeClock);
            G.NodeSetVisted(DiscoveredNodes.top(),true);
        }
        
        for ( Node b: aAdjList ) {
            if ( !G.NodeGetVist(b) ) {
                DiscoveredNodes.push(b);
            }
            
        }

        if ( G.NodeGetVist(DiscoveredNodes.top()) ) {
            if ( G.NodeGetPost(DiscoveredNodes.top()) == 0 ) 
                G.NodeSetPost(DiscoveredNodes.top(), ++NodeClock);
            DiscoveredNodes.pop();
        }
    }
}

void DFS_iterative(Graph &G) {
    NodeClock = 0;
    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            exploreIt(G, a);
        }
    }
}


bool checkAnswers (vector<vector<int>> answers, Graph g ) {
    bool passed = true;
    for ( size_t i = 0; i < g.num_nodes(); i++ ) {
        Node a = g.getNode(i);
        vector<int> anw = answers[i];

        if ( a.getPre() != anw[0] && a.getPost() != anw[1] ) {
            cout<<g.NodeGetName(a)<<":"<<endl;
            cout<<"Graph Pre: " <<a.getPre()<<" Post: "<<a.getPost()<<endl;
            cout<<"Answer: Pre: "<<anw[0]<<" Post: "<<anw[1]<<endl<<endl;
            passed = false;
        }
    }
    cout<<endl;
    return passed;
}

bool testSCC () {
    bool passed = true;
    
    Graph g("test_scc.txt");
    DFS_recursive(g);
    vector< vector<int> > answers = {
        {1,6}, {2,5}, {3,4}, {7,8}, {9,18}, {10,17}, {11,16}, {12,15}, {13,14} 
    };
    passed = checkAnswers(answers, g);
    
    g.Reset();
    DFS_iterative(g);
    passed = checkAnswers(answers,g);

    //             a       b       c       d       e         f      h        g       j        i        k                
    answers = { {1,22}, {2,21}, {3,18}, {15,16}, {14,17}, {19,20}, {4,13}, {5,12}, {6,11}, {7,10}, {8,9} };

    Graph g1("test_undir.txt");
    DFS_recursive(g1);
    passed = checkAnswers(answers, g1);
    cout<<g1<<endl;
    g1.Reset();

    //             a       b       c       d    e         f      h        g       j        i        k                
    answers = { {1,22}, {3,20}, {4,19}, {6,7}, {5,8}, {2,21}, {9,18}, {10,17}, {11,16}, {13,14}, {12,15} };
    DFS_iterative(g1);
    passed = checkAnswers(answers,g1);
    cout<<g1;

    Graph g2("test_acyc.txt");
    answers = { {1,8}, {2,7}, {3,6}, {4,5} };
    DFS_recursive(g2);
    passed = checkAnswers(answers, g2);
    g2.Reset();

    answers = { {1,8}, {4,5}, {2,7}, {3,6} };
    DFS_iterative(g2);
    passed = checkAnswers(answers, g2);
    cout<<g2;


    return passed;

    // for ( size_t i = 0; i < g.num_nodes(); i++ ) {
    //     Node a = g.getNode(i);
    //     vector<int> anw = answers[i];

    //     if ( a.getPre() != anw[0] && a.getPost() != anw[1] ) {
    //         passed = false;
    //     }
    // }
}

bool testall() {
    bool passed = true;
    if ( !testSCC() ) {
        passed =false;
    }

    return passed;
}

int main() {
    bool passed = testall();
    
    if ( !passed ) {
        cout<<"Tests have failed :("<<endl;
    }
    else {
        cout<<"Tests are succesfully!"<<endl;
    }
    // cout<<g;
    
}


//---------------------------------------------------------------

/*** R
 require("igraph")



#Plot the data read from graph file.
graph.plot <- function(links){
    net <- graph_from_data_frame( d=links, directed=T )
    plot(net, vertex.size=30, vertex.label.cex=2)
    
    return (NULL)
}

#Read the graph file, return the inside data. An option to plot or not plot by specifying plot parameter.
graph.from.file <- function(input.file, plot=TRUE){
    ##
    links <- read.table(input.file, header = FALSE, sep = '\t', quote = "", stringsAsFactors = FALSE)
    colnames(links) <- c("from", "to")
    ##
    if(plot){
        graph.plot(links)
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

random.graph(10, 10, "test_small.txt")
random.graph(10, 30, "test_mid.txt" )
random.graph(50000, 500000, "test_midder.txt")
random.graph(100000, 1000000, "test_big.txt")
random.graph(1000000, 2000000, "test_really_big.txt")

testall()

runtimeMidder <- system.time(testMidder())[["user.self"]]

runtimeBig <- system.time(testBig())[["user.self"]]

runtimeReallyBig <- system.time(testReallyBig())[["user.self"]]

if ( !graph.compare("test_small.txt", "test_small_output.txt", plot = TRUE) ) {
    exit("Test Failed")
}

if ( !graph.compare("test_mid.txt", "test_mid_output.txt", plot = TRUE) ) {
    exit("Test Failed")
}

if ( !graph.compare("test_midder.txt", "test_midder_output.txt", plot = FALSE) ) {
    exit("Test Falied")
}

if ( !graph.compare("test_big.txt", "test_big_output.txt", plot = FALSE) ) {
    exit("Test Failed")
}

if ( !graph.compare("test_really_big.txt", "test_really_big_output.txt", plot = FALSE) ) {
    exit("Test Failed")
}

# Comment this out if you do not want output files to be deleted 
# removeFiles()


 */