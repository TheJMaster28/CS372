#include "Lab7.h"


// [[Rcpp::plugins(cpp11)]]

double infin () {
    return numeric_limits<double>::infinity();
}

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
    
    // create queue
    queue<Node> Q;

    // set source node to be 0
    distance(s,G,0);

    // add to queue
    Q.push(s);

    while (!Q.empty()) {
        // grab node on the front of the queue and delete it from the queue
        Node u = Q.front();
        Q.pop();

        // go through each node's adjecnt node and add it to the queue and set distance from source node
        for ( Node v : G.getAdjNodes(u) ) {
            if ( distance(v,G) == infin() ) {
                Q.push(v);
                distance(v,G,distance(u,G)+1);
            }
        }
        
    }

}

bool testBFS(string s,int source,  vector<double> answer) {
    Graph g1(s);
    Node s2 = g1.getNode(source);
    
    BFS(g1,s2);
    cout<<g1;

    vector<Node> nodeList;
    for ( size_t i =0; i < g1.num_nodes(); i++ ) {
        nodeList.push_back(g1.getNode(i));
    }

    if ( nodeList.size() != answer.size() ) {
        return false;
    }

    for ( size_t i = 0; i < answer.size(); i++) {
        if ( answer[i] != g1.NodeGetDistance(nodeList[i])){
            return false;
        }
    }
    return true;
}

//[[Rcpp::export]]
void testBFS(string s) {
    Graph g1(s);
    Node s2 = g1.getNode(0);
    
    BFS(g1,s2);
    
}

//[[Rcpp::export]]
bool testall() {

    if ( !testBFS("test_0.txt",0, {0,1,2,1})) {
        cout<<"Test 1 has failed"<<endl;
        return false;
    }

    if ( !testBFS("test_1.txt",0, {0,1,2,3,2,2,1,infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin()})) {
        cout<<"Test 2 has failed"<<endl;
        return false;
    }

    if ( !testBFS("test_2.txt",0, {0,1,2,2,3,2,infin(),infin(),infin(),infin(),infin()})) {
        cout<<"Test 3 has failed"<<endl;
        return false;
    }

    if ( !testBFS("test_3.txt",0, {0,1,2,infin(),infin(),2,infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin(),infin()})) {
        cout<<"Test 4 has failed"<<endl;
        return false;
    }

    if ( !testBFS("test_4.txt",6, {4,3,3,2,1,2,0,2,3,1})) {
        cout<<"Test 5 has failed"<<endl;
        return false;
    }

    return true;
}

int main () {

    if ( !testall() ) {
        cout<<"Tests failed :("<<endl;
    } else {
        cout<<"Tests were succesfully"<<endl;
    }
    
}


//----------------------------------------------------------------------------------------------------

/*** R
require("igraph")

require("ggplot2")
require("gridExtra")
require("grid")
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

numberNodes <- c( 25000,50000,70000,100000, 110000, 120000, 130000, 150000, 170000, 200000  )

random.graph(numberNodes[1], 1000000, "test_node_1.txt")
random.graph(numberNodes[2], 1000000, "test_node_2.txt")
random.graph(numberNodes[3], 1000000, "test_node_3.txt")
random.graph(numberNodes[4], 1000000, "test_node_4.txt")
random.graph(numberNodes[5], 1000000, "test_node_5.txt")
random.graph(numberNodes[6], 1000000, "test_node_6.txt")
random.graph(numberNodes[7], 1000000, "test_node_7.txt")
random.graph(numberNodes[8], 1000000, "test_node_8.txt")
random.graph(numberNodes[9], 1000000, "test_node_9.txt")
random.graph(numberNodes[10],1000000, "test_node_10.txt")




runtimeNode <- 1:10
fileNames <- c("test_node_1.txt", "test_node_2.txt", "test_node_3.txt", "test_node_4.txt", "test_node_5.txt", "test_node_6.txt", "test_node_7.txt", "test_node_8.txt", "test_node_9.txt", "test_node_10.txt")

for ( i in 1:10 ) {
    
    
    runtimeNode[i] <- system.time(testBFS(fileNames[i]))[["user.self"]]
    
}




random.graph(10000,  125000, "test_edge_1.txt")
random.graph(10000,  250000, "test_edge_2.txt")
random.graph(10000,  300000, "test_edge_3.txt")
random.graph(10000,  400000, "test_edge_4.txt")
random.graph(10000,  500000, "test_edge_5.txt")
random.graph(10000,  600000, "test_edge_6.txt")
random.graph(10000,  650000, "test_edge_7.txt")
random.graph(10000,  750000, "test_edge_8.txt")
random.graph(10000,  850000, "test_edge_9.txt")
random.graph(10000,  1000000, "test_edge_10.txt")


numberOfEdges <- c(125000,250000,300000, 400000, 500000,600000, 650000, 750000, 850000, 1000000 )


runtimeEdge <- 1:10
fileNames <- c("test_edge_1.txt", "test_edge_2.txt", "test_edge_3.txt", "test_edge_4.txt", "test_edge_5.txt", "test_edge_6.txt", "test_edge_7.txt", "test_edge_8.txt", "test_edge_9.txt", "test_edge_10.txt")

for ( i in 1:10 ) {
    
    runtimeEdge[i] <- system.time(testBFS(fileNames[i]))[["user.self"]]
    
}

graph.from.file("test_0.txt", plot=TRUE, direct=T) 
graph.from.file("test_1.txt", plot=TRUE, direct=T) 
graph.from.file("test_2.txt", plot=TRUE, direct=T) 
graph.from.file("test_3.txt", plot=TRUE, direct=T) 
graph.from.file("test_4.txt", plot=TRUE, direct=T) 



#plot(numberNodes, runtimeRnode, type="b", xlab="number of nodes", ylab="runtime (second)")





dataNodes <- data.frame(
    runtime = c(runtimeNode),
    graphType = c(rep(c("Node Runtime"), each=5)), 
    x = c(numberNodes)
    
)




pN <- ggplot(dataNodes, aes(x,runtime,group=graphType)) +
    geom_point(color='blue',aes( shape = graphType)) +
    geom_line( color='blue',aes( linetype = graphType) ) 
pN <- pN + labs(x="Number of Nodes", y="Runtime(seconds)")
pN <- pN + ggtitle("BFS on Function of Number of Nodes")


grid.arrange(pN, nrow=1)


dataEdges <- data.frame(
    runtime = c(runtimeEdge),
    graphType = c(rep(c("Edge Runtime"), each=5)),
    x = c(numberOfEdges)
    
)



pE <- ggplot(dataEdges, aes(x,runtime,group=graphType)) +
    geom_point(color='red', aes(shape = graphType)) +
    geom_line( color='red', aes( linetype = graphType)) 
pE <- pE + labs(x="Number of Edges", y="Runtime(seconds)")
pE <- pE + ggtitle("BFS on Function on Number of Edges")


grid.arrange(pE, nrow=1)

*/