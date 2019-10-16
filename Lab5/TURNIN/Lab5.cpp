// Jeffrey Lansford
// Lab 5
// 10-16-2019
#include "Lab5.h"
#include <stack>
// [[Rcpp::plugins(cpp11)]]


int NodeClock = 0;

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


// compares answers and Graph info to see if it is correct
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
    
    // Test Case 1
    cout<<"Test Case 1\n"<<endl;
    Graph g("test_dir.txt");
    cout<<"Recursive DFS"<<endl;
    DFS_recursive(g);
    vector< vector<int> > answers = {
        {1,6}, {2,5}, {3,4}, {7,8}, {9,18}, {10,17}, {11,16}, {12,15}, {13,14} 
    };
    passed = checkAnswers(answers, g);
    
    g.Reset();
    cout<<"Iterative DFS"<<endl;
    DFS_iterative(g);
    passed = checkAnswers(answers,g);

    // Test Case 2
    cout<<"Test Case 2\n"<<endl;
    answers = { {1,22}, {2,21}, {3,18}, {15,16}, {14,17}, {19,20}, {4,13}, {5,12}, {6,11}, {7,10}, {8,9} };

    Graph g1("test_undir.txt");
    cout<<"Recursive DFS"<<endl;
    DFS_recursive(g1);
    passed = checkAnswers(answers, g1);
    // cout<<g1<<endl;
    g1.Reset();

    answers = { {1,22}, {3,20}, {4,19}, {6,7}, {5,8}, {2,21}, {9,18}, {10,17}, {11,16}, {13,14}, {12,15} };
    cout<<"Iterative DFS"<<endl;
    DFS_iterative(g1);
    passed = checkAnswers(answers,g1);
    // cout<<g1;

    // Test Case 3
    cout<<"Test Case 3\n"<<endl;
    Graph g2("test_acyc.txt");
    answers = { {1,8}, {2,7}, {3,6}, {4,5} };
    cout<<"Recursive DFS"<<endl;
    DFS_recursive(g2);
    passed = checkAnswers(answers, g2);
    g2.Reset();

    answers = { {1,8}, {4,5}, {2,7}, {3,6} };
    cout<<"Iterative DFS"<<endl;
    DFS_iterative(g2);
    passed = checkAnswers(answers, g2);
    // cout<<g2;

    // Test Case 4
    cout<<"Test Case 4\n"<<endl;
    Graph g3("test_ncyc.txt");
    answers = { {1,18}, {2,17}, {3,12}, {13,16}, {4,11}, {14,15}, {7,10}, {8,9}, {5,6} };
    cout<<"Recursive DFS"<<endl;
    DFS_recursive(g3);
    passed = checkAnswers(answers, g3);
    // cout<<g3;
    g3.Reset();

    answers = { {1,18}, {2,17}, {15,16}, {3,14}, {12,13}, {4,11}, {5,10}, {6,9}, {7,8} };
    cout<<"Iterative DFS"<<endl;
    DFS_iterative(g3);
    passed = checkAnswers(answers, g3);
    // cout<<g3;

    // Test Case 5
    cout<<"Test Case 5\n"<<endl;
    Graph g4("test_conn.txt");
    answers = { {1,8}, {2,7}, {3,4}, {9,16}, {10,13}, {5,6}, {17,20}, {18,19}, {11,12}, {21,22}, {23,24}, {25,30}, {31,32}, {14,15}, {33,34}, {26,29}, {27,28}  };
    cout<<"Recursive DFS"<<endl;
    DFS_recursive(g4);
    passed = checkAnswers(answers, g4);
    // cout<<g4;
    g4.Reset();

    answers = { {1,8}, {2,7}, {5,6}, {9,16}, {12,15}, {3,4}, {17,20}, {18,19}, {13,14}, {21,22}, {23,24}, {25,30}, {31,32}, {10,11}, {33,34}, {26,29}, {27,28} };
    cout<<"Iterative DFS"<<endl;
    DFS_iterative(g4);
    passed = checkAnswers(answers, g4);
    // cout<<g4;

    return passed;

}

//[[Rcpp::export]]
void testDFSRecursive(string s) {
    Graph g(s);
    DFS_recursive(g);
}

//[[Rcpp::export]]
void testDFSIterative(string s) {
    Graph g(s);
    DFS_iterative(g);
}




//[[Rcpp::export]]
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

    // Graph g("test_big.txt");
    // DFS_recursive(g);
    // cout<<"blah"<<endl;
    // cout<<g;
    
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

#if ( !testall() ) {
#    exit (1);
#}
random.graph(12500,  100000, "test_node_1.txt")
random.graph(25000,  100000, "test_node_2.txt")
random.graph(50000,  100000, "test_node_3.txt")
random.graph(100000, 100000, "test_node_4.txt")

numberNodes <- c(1250,2500,5000,10000 )

runtimeRnode <- 1:4
runtimeInode <- 1:4
fileNames <- c("test_node_1.txt", "test_node_2.txt", "test_node_3.txt", "test_node_4.txt")

for ( i in 1:4 ) {
    
    runtimeRnode[i] <- system.time(testDFSRecursive(fileNames[i]))[["user.self"]]
    runtimeInode[i] <- system.time(testDFSIterative(fileNames[i]))[["user.self"]]
    
}

random.graph(10000,  125000,"test_edge_1.txt")
random.graph(10000,  250000, "test_edge_2.txt")
random.graph(10000,  500000, "test_edge_3.txt")
random.graph(10000,  1000000, "test_edge_4.txt")

numberOfEdges <- c(125000,250000,500000,1000000)

runtimeRedge <- 1:4
runtimeIedge <- 1:4
fileNames <- c("test_edge_1.txt", "test_edge_2.txt", "test_edge_3.txt", "test_edge_4.txt")

for ( i in 1:4 ) {
    
    runtimeRedge[i] <- system.time(testDFSRecursive(fileNames[i]))[["user.self"]]
    runtimeIedge[i] <- system.time(testDFSIterative(fileNames[i]))[["user.self"]]
    
}

graph.from.file("test_dir.txt", plot=TRUE, direct=T) 
graph.from.file("test_undir.txt", plot=TRUE, direct=T) 
graph.from.file("test_acyc.txt", plot=TRUE, direct=T) 
graph.from.file("test_ncyc.txt", plot=TRUE, direct=T) 
graph.from.file("test_conn.txt", plot=TRUE, direct=T) 



#plot(numberNodes, runtimeRnode, type="b", xlab="number of nodes", ylab="runtime (second)")

data <- data.frame(
    runtime = c(runtimeRnode, runtimeInode),
    graphType = c(rep(c("Recursive"), each=4), c(rep(c("Iterative"), each=4))),
    x = c(numberNodes)
    
)
p <- ggplot(data, aes(x,runtime,group=graphType)) +
    geom_point(aes(color=graphType, shape = graphType)) +
    geom_line( aes( color=graphType, linetype = graphType) ) +
    scale_linetype_manual(values=c( "solid", "dashed","dashed", "solid" ))
p <- p + labs(x="Number of Nodes", y="Runtime(seconds)")
p <- p + ggtitle("DFS Function on Number of Nodes")


data <- data.frame(
    runtime = c(runtimeRedge, runtimeIedge),
    graphType = c(rep(c("Recursive"), each=4), c(rep(c("Iterative"), each=4))),
    x = c(numberOfEdges)
    
)

p2 <- ggplot(data, aes(x,runtime,group=graphType)) +
    geom_point(aes(color=graphType, shape = graphType)) +
    geom_line( aes( color=graphType, linetype = graphType) ) +
    scale_linetype_manual(values=c( "solid", "dashed","dashed", "solid" ))
p2 <- p2 + labs(x="Number of Edges", y="Runtime(seconds)")
p2 <- p2 + ggtitle("DFS Function on Number of Edges")

grid.arrange(p, nrow=1)

grid.arrange(p2, nrow=1)


#random.graph(10, 10, "test_small.txt")
#random.graph(10, 30, "test_mid.txt" )
#random.graph(50000, 500000, "test_midder.txt")
#random.graph(100000, 1000000, "test_big.txt")
#random.graph(1000000, 2000000, "test_really_big.txt")

#testall()

#runtimeMidder <- system.time(testMidder())[["user.self"]]

#runtimeBig <- system.time(testBig())[["user.self"]]

#runtimeReallyBig <- system.time(testReallyBig())[["user.self"]]

#if ( !graph.compare("test_small.txt", "test_small_output.txt", plot = TRUE) ) {
#    exit("Test Failed")
#}

#if ( !graph.compare("test_mid.txt", "test_mid_output.txt", plot = TRUE) ) {
#    exit("Test Failed")
#}

#if ( !graph.compare("test_midder.txt", "test_midder_output.txt", plot = FALSE) ) {
#    exit("Test Falied")
#}

#if ( !graph.compare("test_big.txt", "test_big_output.txt", plot = FALSE) ) {
#    exit("Test Failed")
#}

#if ( !graph.compare("test_really_big.txt", "test_really_big_output.txt", plot = FALSE) ) {
#    exit("Test Failed")
#}

# Comment this out if you do not want output files to be deleted 
# removeFiles()


 */