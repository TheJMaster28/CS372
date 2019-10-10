#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
using namespace std;

class Node {

    private:
        string m_name;
        int m_id;
        int pre;
        int post;
        bool visted;

    public:
        Node() { 
            pre = 0;
            post = 0;
        };
        Node( const string & name, int id)
            {m_name = name, m_id = id;};
        int id () const { return m_id;};
        const string & name() const { return m_name;};
        void setPre(int i) {pre = i;}
        void setPost( int i ) {post = i;}
        void setVisted( bool i) {visted = i;}
        int getPre() const {return pre;}
        int getPost() const {return post;}
        bool getVisted() const {return visted;}
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

        int NodeGetPre( Node & a) { getNode(a.id()).getPre(); }

        int NodeGetPost( Node & a) { getNode(a.id()).getPost(); }
        
        bool NodeGetVist( Node & a) { getNode(a.id()).getVisted(); }

        void NodeSetPre ( Node & a, int i) { getNode(a.id()).setPre(i); }

        void NodeSetPost ( Node & a, int i) { getNode(a.id()).setPost(i); }

        void NodeSetVisted( Node & a, bool i) { getNode(a.id()).setVisted(i); }

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
        Node a = g.getNode(i);
        out << a.name()<< " Pre: "<<a.getPre()<<" Post: "<<a.getPost()<<"\n";
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