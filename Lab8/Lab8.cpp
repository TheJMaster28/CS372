// #include "Lab8.h"
#include "BinaryHeap.h"

// [[Rcpp::plugins(cpp11)]]

double infin() { return numeric_limits<double>::infinity(); }

double distance(Node &a, Graph &G) { return G.NodeGetDistance(a); }

void distance(Node &a, Graph &G, double dis) { G.NodeSetDistance(a, dis); }

list<Node> make_Queue(Graph &G) {
    list<Node> Pqueue;
    for (size_t i = 0; i < G.num_nodes(); i++) {
        Pqueue.push_back(G.getNode(i));
    }
    return Pqueue;
}

Node deleteMin(list<Node> &H, Graph &G) {
    list<Node>::iterator minN;
    double min = infin();

    // list<Node>::iterator k = bList.begin(); k != bList.end(); k++
    for (list<Node>::iterator a = H.begin(); a != H.end(); a++) {
        if (min >= distance(*a, G)) {
            min = distance(*a, G);
            minN = a;
        }
    }

    H.erase(minN);
    return *minN;
}

void Dijkstra_list(Graph &G, Node &s) {
    G.Reset();
    distance(s, G, 0);
    list<Node> H = make_Queue(G);
    while (!H.empty()) {
        Node u = deleteMin(H, G);
        for (Node v : G.getAdjNodes(u)) {
            double uPlus = distance(u, G) + G.getWeight(u, v);
            if (distance(v, G) > uPlus) {
                distance(v, G, uPlus);
            }
        }
    }
}

void Dijkstra_heap(Graph &G, Node &s) {
    G.Reset();
    distance(s, G, 0);
    BinaryHeap H(G);
    while (!H.empty()) {
        Node u = H.deletMin();
        for (Node v : G.getAdjNodes(u)) {
            double uPlus = distance(u, G) + G.getWeight(u, v);
            if (distance(v, G) > uPlus) {
                distance(v, G, uPlus);
                H.decreseKey(H.getIndex(v), uPlus);
            }
        }
    }
}

//[[Rcpp::export]]
void test_List(string name) {
    cout << "Creating Graph for List" << endl;
    Graph g(name);
    Node source = g.getNode(0);
    cout << "Start List" << endl;
    Dijkstra_list(g, source);
    cout << "Done List" << endl;
}

//[[Rcpp::export]]
void test_Heap(string name) {
    cout << "Creating Graph for Heap" << endl;
    Graph g(name);
    Node source = g.getNode(0);
    cout << "Start Heap" << endl;
    Dijkstra_heap(g, source);
    cout << "Done Heap" << endl;
}

//[[Rcpp::export]]
void test_Graph(string name) {
    Graph g(name);
}

bool test(string name, vector<double> answer) {
    Graph g(name);
    Node source = g.getNode(0);
    Dijkstra_list(g, source);
    for (size_t i = 0; i < answer.size(); i++) {
        if (answer[i] != g.getNode(i).getDistance()) {
            cout << g;
            cout << "List is wrong" << endl;
            return false;
        }
    }

    g.Reset();
    source = g.getNode(0);
    Dijkstra_heap(g, source);

    for (size_t i = 0; i < answer.size(); i++) {
        if (answer[i] != g.getNode(i).getDistance()) {
            cout << g;
            cout << "Heap is wrong" << endl;
            return false;
        }
    }
    return true;
}

//[[Rcpp::export]]
bool testall() {
    if (!test("test_0.txt", vector<double>{0, 3.4, 6.5, 8.4, 11.4, 5.4})) {
        return false;
    }
    if (!test("test_1.txt", vector<double>{0, 2, 4, 3})) {
        return false;
    }
    if (!test("test_2.txt", vector<double>{0, 120.58, infin(), infin(), infin(), infin()})) {
        return false;
    }
    if (!test("test_3.txt", vector<double>{0, 1, 7, 5})) {
        return false;
    }
    if (!test("test_4.txt", vector<double>{0, 35.7, 39.4, 25.7, 65.157})) {
        return false;
    }

    return true;
}

int main() {
    if (!testall()) {
        cout << "Tests Failed" << endl;
    }

    return 0;
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
    net <- graph_from_data_frame( d=links, directed=direct  )
    is_weighted(net)
    plot(net, vertex.size=30, vertex.label.cex=2, edge.label= links$weight )
    
    return (NULL)
}

#Read the graph file, return the inside data. An option to plot or not plot by specifying plot parameter.
graph.from.file <- function(input.file, plot=TRUE, direct=T){
    ##
    links <- read.table(input.file, header = FALSE, sep = '\t', quote = "", stringsAsFactors = FALSE)
    colnames(links) <- c("from", "to", "weight")
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
    num <- sample(1:10, number.of.nodes, TRUE)
    
    parent.nodes <- c(nodes, nodes[sample(c(1:number.of.nodes), number.of.edges-number.of.nodes, TRUE)])
    parent.nodes <- parent.nodes[sample(c(1:length(parent.nodes)), length(parent.nodes), FALSE)]
    
    child.nodes <- c(nodes, nodes[sample(c(1:number.of.nodes), number.of.edges-number.of.nodes, TRUE)])
    child.nodes <- child.nodes[sample(c(1:length(child.nodes)), length(child.nodes), FALSE)]
    
    weight <- c(num, num[sample(c(1:number.of.nodes), number.of.edges-number.of.nodes, TRUE)])
    weight <- weight[sample(c(1:length(weight)), length(weight), FALSE)]

    links <- data.frame(from = parent.nodes, to = child.nodes, w = weight)
    write.table(links, output.file, append = FALSE, sep = '\t', quote = FALSE, row.names = FALSE, col.names = FALSE)
    message("Scrpt is Done")
}

if ( !testall() ) {
    exit (1);
}


numberNodes <- c( 2500,5000,7000,10000, 11000, 12000, 13000, 15000, 17000, 20000  )



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




runtimeNodeL <- 1:10
runtimeNodeH <- 1:10
fileNames <- c("test_node_1.txt", "test_node_2.txt", "test_node_3.txt", "test_node_4.txt", "test_node_5.txt", "test_node_6.txt", "test_node_7.txt", "test_node_8.txt", "test_node_9.txt", "test_node_10.txt")

for ( i in 1:10 ) {
  
    runtimeNodeL[i] <- system.time(test_List(fileNames[i]))[["user.self"]]
    runtimeNodeH[i] <- system.time(test_Heap(fileNames[i]))[["user.self"]]
    
}




numberOfEdges <- c(125000,250000,300000, 400000, 500000,600000, 650000, 750000, 850000, 1000000 )



random.graph(10000,  numberOfEdges[1], "test_edge_1.txt")
random.graph(10000,  numberOfEdges[2], "test_edge_2.txt")
random.graph(10000,  numberOfEdges[3], "test_edge_3.txt")
random.graph(10000,  numberOfEdges[4], "test_edge_4.txt")
random.graph(10000,  numberOfEdges[5], "test_edge_5.txt")
random.graph(10000,  numberOfEdges[6], "test_edge_6.txt")
random.graph(10000,  numberOfEdges[7], "test_edge_7.txt")
random.graph(10000,  numberOfEdges[8], "test_edge_8.txt")
random.graph(10000,  numberOfEdges[9], "test_edge_9.txt")
random.graph(10000,  numberOfEdges[10], "test_edge_10.txt")




runtimeEdgeL <- 1:10
runtimeEdgeH <- 1:10
fileNames <- c("test_edge_1.txt", "test_edge_2.txt", "test_edge_3.txt", "test_edge_4.txt", "test_edge_5.txt", "test_edge_6.txt", "test_edge_7.txt", "test_edge_8.txt", "test_edge_9.txt", "test_edge_10.txt")

for ( i in 1:10 ) {
    
   
    
    runtimeEdgeL[i] <- system.time(test_List(fileNames[i]))[["user.self"]] 
    
    runtimeEdgeH[i] <- system.time(test_Heap(fileNames[i]))[["user.self"]]
    
}

graph.from.file("test_0.txt", plot=TRUE, direct=T) 
graph.from.file("test_1.txt", plot=TRUE, direct=T) 
graph.from.file("test_2.txt", plot=TRUE, direct=T) 
graph.from.file("test_3.txt", plot=TRUE, direct=T) 
graph.from.file("test_4.txt", plot=TRUE, direct=T) 



#plot(numberNodes, runtimeRnode, type="b", xlab="number of nodes", ylab="runtime (second)")





dataNodes <- data.frame(
    numberOfNodes = numberNodes,
    graphType = c(rep(c("List"), each=10) , rep(c("Heap"), each=10)),
    runtime = c(runtimeNodeL, runtimeNodeH)
    
)




pN <- ggplot(dataNodes, aes(numberOfNodes,runtime,group=graphType)) +
    geom_point(aes( color = graphType )) +
    geom_line( aes( color=graphType,linetype = graphType) ) 
pN <- pN + labs(numberOfNodes="Number of Nodes", y="Runtime(seconds)")
pN <- pN + ggtitle("Dijkstra Algorthim on Function of Nodes")


grid.arrange(pN, nrow=1)


dataEdges <- data.frame(
  numberOfEdges = numberOfEdges,
  graphType = c(rep(c("List"), each=10) , rep(c("Heap"), each=10)),
  runtime = c(runtimeEdgeL, runtimeEdgeH)
    
)



pE <- ggplot(dataEdges, aes(numberOfEdges,runtime,group=graphType)) +
    geom_point( aes(color = graphType)) +
    geom_line( aes( color = graphType,linetype = graphType)) 
pE <- pE + labs(numberOfEdges="Number of Edges", y="Runtime(seconds)")
pE <- pE + ggtitle("Dijkstra Algorthim on Function of Edges")


grid.arrange(pE, nrow=1)

*/