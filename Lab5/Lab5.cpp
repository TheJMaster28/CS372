#include "Lab5.h"

// [[Rcpp::plugins(cpp11)]]


int NodeClock = 0;

void explore ( Graph &G, Node &v) {
    v.setVisted(true);
    v.setPre(++NodeClock);
    G.addNode(v);
    list<Node> vAdjNodes = G.getAdjNodes(v);
    for ( Node i : vAdjNodes ) {
        Node a = G.getNode(i.id());
        if ( !a.getVisted() ) {
            explore(G, a);
        }

    }
    v.setPost(++NodeClock);
    G.addNode(v);
}

void DFS_recursive(Graph &G) {
    NodeClock = 0;
    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        a.setVisted(false);
        G.addNode(a);
    }

    for ( size_t i = 0; i < G.num_nodes(); i++ ) {
        Node a = G.getNode(i);
        if ( !a.getVisted() ) {
            explore(G, a);
        }
    }
}

void DFS_iterative(Graph &G) {
    NodeClock = 0;
}


bool testSCC () {
    bool passed = true;
    Graph g("test_scc.txt");
    DFS_recursive(g);
    vector< vector<int> > answers = {
        {1,6}, {2,5}, {3,4}, {7,8}, {9,18}, {10,17}, {11,16}, {12,15}, {13,14} 
    };

    for ( size_t i = 0; i < g.num_nodes(); i++ ) {
        Node a = g.getNode(i);
        vector<int> anw = answers[i];

        if ( a.getPre() != anw[0] && a.getPost() != anw[1] ) {
            passed = false;
        }
    }
    return passed;
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