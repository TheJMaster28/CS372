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
                cout<<NewFileName<<endl; 
                save(NewFileName.append("_output.txt"));
            };

        void addEdge ( const Node & a, const Node & b) {
            // cout<<"BLAH"<<endl;

            list<Node> l = m_adjList[a.id()];
            // l.push_back(a);
            l.push_back(b);
            // m_adjList.resize;
            m_adjList[a.id()] = l;
            
        }

        void addNode( const Node & a ) { 
            m_nodes[a.id()] = a;};

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
            size_t edgeId = 0;

            // using hash table
            unordered_map<string, size_t > nodeMap;
            // unordered_map< string, string > edgeMap;
            int edgeNumber = 0; 

            if ( myFile.is_open()) {
                
                while ( myFile >> c1 >> c2) {
                    
                    // get total nodes first
                    
                    unordered_map< string, size_t >::const_iterator nodeA = nodeMap.find(c1);

                    
                    
                    if ( nodeA == nodeMap.end() ) {
                        pair< string, size_t > node1 ( c1, id++ );
                        nodeMap.insert(node1);

                    }
                    
                    unordered_map< string, size_t >::const_iterator nodeB = nodeMap.find(c2);

                    if ( nodeB == nodeMap.end() ) {
                        pair< string, size_t > node2 ( c2, id++ );
                        nodeMap.insert(node2);
                    }
                    // edgeNumber++;
                    // unordered_map< string, string >::const_iterator findEdge = edgeMap.find( c1 );

                    // if ( findEdge == edgeMap.end() || ( findEdge->second.compare(c2) != 0 )) {
                    //     edgeMap.insert( {c1, c2} );
                    // }
                    


                    // findNode(c1,nodeMap);
                    // findNode(c2,nodeMap);
                    // edgeList.push_back(c1);
                    // edgeList.push_back(c2);

                    // if ( !findNode(c1,nodeList) ) {
                    //     nodeList.push_back(c1);
                    //     totalNodes++;
                    // }

                    // if ( !findNode(c2,nodeList) ) {
                    //     nodeList.push_back(c2);
                    //     totalNodes++;
                    // }
                }

                m_nodes.resize(nodeMap.size());
                m_adjList.resize(nodeMap.size());

                for ( pair <string, size_t> node : nodeMap ) {
                    Node a ( node.first, node.second );
                    addNode(a);
                }
                myFile.close();
                myFile.open(file);
                while ( myFile >> c1 >> c2) {
                    
                    unordered_map< string, size_t >::const_iterator nodeA = nodeMap.find(c1);

                    unordered_map< string, size_t >::const_iterator nodeB = nodeMap.find(c2);
                    
                    Node a = getNode(nodeA->second);
                    Node b = getNode(nodeB->second);

                    addEdge(a,b);
                }


                // for ( pair < string, string > node : edgeMap ) {

                //     unordered_map< string, size_t >::const_iterator nodeA = nodeMap.find(node.first);

                //     unordered_map< string, size_t >::const_iterator nodeB = nodeMap.find(node.second);

                //     Node a = getNode(nodeA->second);
                //     Node b = getNode(nodeB->second);
                //     addEdge(a,b);
                    
                // }

                // m_nodes.resize(nodeMap.size());

                // for ( size_t i = 0; i < nodeMap.size(); i++ ) {

                //     m_nodes[i] = nodeMap[i]

                // }
                
                // m_nodes.resize(totalNodes);
                
                // for ( size_t i = 0; i < nodeList.size() ; i++) {
                //     Node a ( nodeList[i], id );
                //     edgeNum.push_back(id);
                //     id++;
                //     // Node b ( nodeList[i], id++ );
                //     addNode(a);
                //     // addNode(b);
                // }
                
                // m_adjList.resize(edgeList.size());
                
                // int place = 0;
                
                // for (size_t i = 0; i < edgeList.size(); i++) {
                    

                //     int placeHolder = findNodeNum(edgeList[i], nodeList);
                //     Node a = getNode(placeHolder);
                //     int placeHolder1 = findNodeNum(edgeList[++i], nodeList);
                //     Node b = getNode(placeHolder1);
                //     addEdge(a,b, placeHolder);
                    
                // }
            }
        }

        void save( const string & file ) const{
            ofstream MyFile;
            MyFile.open(file);


            for ( int i = 0; i < m_nodes.size(); i ++ ) {
                
                Node a = m_nodes[i];

                list<Node> bList = getAdjNodes(a);

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

bool testSmall() {
    bool passed = true;
    Graph g("test_small.txt");
    // cout<<g;
    return passed;
}

bool testBig() {
    bool passed = true;
    Graph g("test_big.txt");
    // cout<<g;
    return passed;
}

bool testall() {
    bool passed = false; 
    if ( testSmall() ) {
        passed = true;
    } 
    return passed;
    
}

int main() {
    bool passed = testall();
    if ( passed ) {
        cout<<"Tests are succesfully!"<<endl;
    }
    else {
        cout<<"Tests Failed! :("<<endl;
    }
    return 0;
}


//---------------------------------------------------------------

/*** R
 require("igraph")
 links <- data.frame(
    from=c("a", "a", "c", "d", "e"),
    to=c("d","b","b","c","a" ))
net <- graph_from_data_frame(d=links, directed=T)
plot(net, vertex.size=30, vertex.label.cex=2)

net <- graph_from_data_frame(d=links, directed=F)
plot(net, vertex.size=30, vertex.label.cex=2)

 */