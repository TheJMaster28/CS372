#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
using namespace std;

// [[Rcpp::plugins(cpp11)]]

class Node {

    private:
        string m_name;
        int m_id;

    public:
        Node() {};
        Node( const string & name, int id)
            {m_name = name, m_id = id;};
        int id () const { return m_id;};
        const string & name() const { return m_name;};
};

class Graph {
    private:
        vector< Node > m_nodes;
        vector< list<Node> > m_adjList;

    public:
        Graph ( const string & file )
            { 
                scan(file);
                string NewFileName = file.substr(0,(file.size()-4));
                save(NewFileName.append("_output.txt"));
                cout<<"Done with: "<<NewFileName<<endl;
            };

        void addEdge ( const Node & a, const Node & b) {
            list<Node> l = m_adjList[a.id()];
            l.push_back(b);
            m_adjList[a.id()] = l;
            
        }

        void addNode( const Node & a ) { m_nodes[a.id()] = a; };

        const Node & getNode(size_t i) const { return m_nodes[i]; }

        list<Node> & getAdjNodes ( const Node & a)
            {return m_adjList[a.id()];}
        
        const list<Node> & getAdjNodes ( const Node & a) const
            {return m_adjList[a.id()];}
        
        size_t num_nodes() const { return m_nodes.size(); }

        void scan(const string & file ){
            ifstream myFile;
            myFile.open( file );
            string c1, c2;
            size_t id = 0;

            // using hash table
            unordered_map<string, size_t > nodeMap;
            // unordered_map< string, string > edgeMap; 

            if ( myFile.is_open()) {
                
                // goes through file and adds nodes to a hash table  
                while ( myFile >> c1 >> c2) {
                    // Find node to make sure it is not already found  
                    unordered_map< string, size_t >::const_iterator nodeA = nodeMap.find(c1);
                    
                    // if nout found add to hash table
                    if ( nodeA == nodeMap.end() ) {
                        pair< string, size_t > node1 ( c1, id++ );
                        nodeMap.insert(node1);

                    }
                    
                    unordered_map< string, size_t >::const_iterator nodeB = nodeMap.find(c2);

                    if ( nodeB == nodeMap.end() ) {
                        pair< string, size_t > node2 ( c2, id++ );
                        nodeMap.insert(node2);
                    }
                    
                }

                // resizes Vectors to make inserting elements quicker and have allcated space
                m_nodes.resize(nodeMap.size());
                m_adjList.resize(nodeMap.size());

                // goes through hash table of nodes and adds them to m_nodes vector
                for ( pair <string, size_t> node : nodeMap ) {
                    Node a ( node.first, node.second );
                    addNode(a);
                }

                // reopen file
                myFile.close();
                myFile.open(file);
                
                // goes through file again and adds edges to the m_adjList vector
                while ( myFile >> c1 >> c2) {
                    
                    // find both nodes in hash table
                    unordered_map< string, size_t >::const_iterator nodeA = nodeMap.find(c1);
                    unordered_map< string, size_t >::const_iterator nodeB = nodeMap.find(c2);
                    
                    // get both nodes and add edge
                    Node a = getNode(nodeA->second);
                    Node b = getNode(nodeB->second);
                    addEdge(a,b);
                }
            }
        }

        void save( const string & file ) const {
            // get file for outputing 
            ofstream MyFile;
            MyFile.open(file);

            // goes through each nodes list and writes the edge tab-spaced
            for ( size_t i = 0; i < m_nodes.size(); i ++ ) {
                
                // get adjacent nodes
                Node a = getNode(i);
                list<Node> bList = getAdjNodes(a);

                // goes through list and writes it to output file
                for ( list<Node>::iterator k = bList.begin(); k != bList.end(); k++ ) {
                    MyFile<<a.name()<<"\t"<<k->name()<<endl;
                }
            }
        }

};

std::ostream& operator<<(std::ostream& out, const Graph & g)
{
    out << "Nodes in the graph:" << endl;
    for ( unsigned i=0; i <g.num_nodes(); i++ ) {
        out << g.getNode(i).name() << ",";
    }
    out << endl;
    out << "Adjacency list of the graph :" << endl;
    for ( unsigned i=0; i<g.num_nodes(); i++) {
        out << "Node " << g.getNode(i).name() << ":";
        const list <Node> neighbors = g.getAdjNodes(g.getNode(i));
        for ( list <Node>::const_iterator itr = neighbors.begin(); itr != neighbors.end(); ++itr) {
            out << itr -> name() << ",";
        }
        out << endl;
    }

    return out;
}

//[[Rcpp::export]]
bool test( const string &s) {
    bool passed = true;
    Graph g(s);
    // cout<<g;
    return passed;
}

//[[Rcpp::export]]
bool testBig() {
    bool passed = true;
    if ( !test("test_big.txt") ) {
        passed = false;
    }
    return passed;
}

//[[Rcpp::export]]
bool testReallyBig() {
    bool passed = true;
    if ( !test("test_really_big.txt") ) {
        passed = false;
    }
    return passed;
}

//[[Rcpp::export]]
bool testMidder() {
    bool passed = true;
    if ( !test("test_midder.txt")) {
        passed = false;
    }
    return passed;
}

//[[Rcpp::export]]
bool testall() {

    bool passed = true; 

    if ( !test("test_small.txt") ) {
        passed = false;
    }
    
    if ( !test("test_mid.txt") ) {
        passed = false;
    }
    if ( !test("test_midder.txt") ) {
        passed = false;
    }
    
    if ( !test("test_big.txt") ) {
        passed = false;
    }

    if ( !test("test_really_big.txt") ) {
        passed = false;
    }

    return passed;
    
}



//[[Rcpp::export]]
void removeFiles() {
    remove("test_small_output.txt");
    remove("test_mid_output.txt");
    remove("test_midder_output.txt");
    remove("test_big_output.txt");
    remove("test_really_big_output.txt");
}

int main( int argc, char* argv[]) {


    bool passed = testall();
    if ( passed ) {
        cout<<"Tests are succesfully!"<<endl;
    }
    else {
        cout<<"Tests Failed! :("<<endl;
    }
    if ( argc >= 2 ) {
        string args(argv[1]);
        if ( args.compare("-r") == 0 )
            removeFiles();
    }
    return 0;
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