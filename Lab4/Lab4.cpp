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

        void addEdge ( const Node & a, const Node & b, int place) {
            // cout<<"BLAH"<<endl;

            list<Node> l = m_adjList[place];
            // l.push_back(a);
            l.push_back(b);
            // m_adjList.resize;
            m_adjList[place] = l;
        }

        void addNode( const Node & a ) { 
            m_nodes[a.id()] = a;};

        const Node & getNode(size_t i) const { return m_nodes[i]; }

        list<Node> & getAdjNodes ( const Node & a)
            {return m_adjList[a.id()];}
        
        const list<Node> & getAdjNodes ( const Node & a) const
            {return m_adjList[a.id()];}
        
        size_t num_nodes() const { return m_nodes.size(); }

        bool findNode ( string &name, vector<string> &l ) const {
            bool r = false;
            
            if ( l.empty() ) {
                return r;
            }

            for ( int i = 0; i < l.size(); i ++ ) {
                // cout<<"in list: "<<l[i]<<endl;
                // cout<<"Incoming: "<<name<<endl;
                if ( l[i].compare(name) == 0 ) {
                    
                    r = true;
                    return r;
                }
            }
            return r; 
        }

        int findNodeNum ( string &name, vector<string> &l ) const {
            int r = -1;
            
            if ( l.empty() ) {
                return r;
            }

            for ( int i = 0; i < l.size(); i ++ ) {
                // cout<<"in list: "<<l[i]<<endl;
                // cout<<"Incoming: "<<name<<endl;
                if ( l[i].compare(name) == 0 ) {
                    
                    r = i;
                    return r;
                }
            }
            return r; 
        }

        void scan(const string & file ){
            ifstream myFile;
            myFile.open( file );
            string c1, c2;
            int id = 0;
            int totalNodes = 0;
            vector< string > nodeList; 
            vector< string > edgeList;
            vector< int > edgeNum; 
            
            if ( myFile.is_open()) {
                
                while ( myFile >> c1 >> c2) {

                    // get total nodes first
                    
                    edgeList.push_back(c1);
                    edgeList.push_back(c2);

                    if ( !findNode(c1,nodeList) ) {
                        nodeList.push_back(c1);
                        totalNodes++;
                    }

                    if ( !findNode(c2,nodeList) ) {
                        nodeList.push_back(c2);
                        totalNodes++;
                    }
                }
                
                m_nodes.resize(totalNodes);
                
                for ( size_t i = 0; i < nodeList.size() ; i++) {
                    Node a ( nodeList[i], id );
                    edgeNum.push_back(id);
                    id++;
                    // Node b ( nodeList[i], id++ );
                    addNode(a);
                    // addNode(b);
                }
                
                m_adjList.resize(edgeList.size());
                
                int place = 0;
                
                for (size_t i = 0; i < edgeList.size(); i++) {
                    

                    int placeHolder = findNodeNum(edgeList[i], nodeList);
                    Node a = getNode(placeHolder);
                    int placeHolder1 = findNodeNum(edgeList[++i], nodeList);
                    Node b = getNode(placeHolder1);
                    addEdge(a,b, placeHolder);
                    
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