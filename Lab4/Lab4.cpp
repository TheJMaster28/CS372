#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace std;

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
            };

        void addEdge ( const Node & a, const Node & b) {
            cout<<"BLAH"<<endl;

            list<Node> l;
            l.push_back(a);
            l.push_back(b);
            m_adjList.resize;
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
            int id = 0;
            if ( myFile.is_open()) {
                while ( myFile >> c1 >> c2) {
                    Node a(c1, id);
                    m_nodes.resize(id+1);
                    addNode(a);
                    id++;
                    Node b(c2, id);
                    m_nodes.resize(id+1);
                    addNode(b);
                    id++;
                    
                }
                for ( int j= 0 ; j < id; j++ ) {
                    Node a = getNode(j);
                    addEdge(a,b);
                }
                
            }
        }

        void save( const string & file ) const{
            
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


bool testall() {
    bool passed = true;
    Graph g("format.txt");
    cout<<g;
    return passed;
}

int main() {
    testall();
    return 0;
}


//---------------------------------------------------------------

/*** R
 require("igraph")
 links <- data.frame(
    from=c("a", "a," "c", "d", "e")
    to=c("d","b","b",c","a" ))
net <- graph_from_data_frame(d=links, directed=T)
plot(net, vertex.size=30, vertex.label.cex=2)

net <- graph_from_data_frame(d=links, directed=F)
plot(net, vertex.size=30, vertex.label.cex=2)

 */