#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {
   private:
    string m_name;
    int m_id;
    int pre;
    int post;
    bool visted;
    double distance;

   public:
    Node(){};
    Node(const string &name, int id) {
        m_name = name, m_id = id;
        pre = 0;
        post = 0;
        visted = false;
        distance = numeric_limits<double>::infinity();
    };
    int id() const { return m_id; };
    const string &name() const { return m_name; };
    void setPre(int i) { pre = i; }
    void setPost(int i) { post = i; }
    void setVisted(bool i) { visted = i; }
    void setDistance(double i) { distance = i; }
    int getPre() const { return pre; }
    int getPost() const { return post; }
    bool getVisted() const { return visted; }
    double getDistance() const { return distance; }
};

class Graph {
   private:
    vector<Node> m_nodes;
    vector<list<Node>> m_adjList;
    string fileName;
    // hash table of all weights for each edge
    unordered_map<string, double> weightList;

   public:
    Graph(const string &file) {
        fileName = file;
        scan(file);
        string NewFileName = file.substr(0, (file.size() - 4));
        // not longer need to save file, saves runtime on larger graphs
        // save(NewFileName.append("_output.txt"));
    };

    // add edge with weights now
    void addEdge(Node &a, const Node &b, double weight) {
        list<Node> l = m_adjList[a.id()];
        l.push_back(b);
        m_adjList[a.id()] = l;
        string ab = a.name() + "-" + b.name();
        pair<string, double> abWeight(ab, weight);
        weightList.insert(abWeight);
    }

    string getFileName() { return fileName; }

    void addNode(const Node &a) { m_nodes[a.id()] = a; };

    const Node &getNode(size_t i) const { return m_nodes[i]; }

    void clearAdjNodes(const Node &a) {
        list<Node> empty;
        m_adjList[a.id()] = empty;
    }

    list<Node> &getAdjNodes(const Node &a) { return m_adjList[a.id()]; }

    const list<Node> &getAdjNodes(const Node &a) const {
        return m_adjList[a.id()];
    }

    // get weight from hash table to make it consant time
    const double getWeight(const Node &a, const Node &b) const {
        string ab = a.name() + "-" + b.name();
        unordered_map<string, double>::const_iterator weight = weightList.find(ab);
        return weight->second;
    }

    size_t num_nodes() const { return m_nodes.size(); }

    string NodeGetName(Node &a) { return getNode(a.id()).name(); }

    int NodeGetPre(Node &a) { return getNode(a.id()).getPre(); }

    int NodeGetPost(Node &a) { return getNode(a.id()).getPost(); }

    bool NodeGetVist(Node &a) { return getNode(a.id()).getVisted(); }

    double NodeGetDistance(Node &a) { return getNode(a.id()).getDistance(); }

    void NodeSetPre(Node &a, int i) {
        Node node = getNode(a.id());
        node.setPre(i);
        addNode(node);
    }

    void NodeSetPost(Node &a, int i) {
        Node node = getNode(a.id());
        node.setPost(i);
        addNode(node);
    }

    void NodeSetVisted(Node &a, bool i) {
        Node node = getNode(a.id());
        node.setVisted(i);
        addNode(node);
    }

    void NodeSetDistance(Node &a, double i) {
        Node node = getNode(a.id());
        node.setDistance(i);
        addNode(node);
    }

    void Reset() {
        for (size_t i = 0; i < m_nodes.size(); i++) {
            NodeSetVisted(m_nodes[i], false);
            NodeSetPre(m_nodes[i], 0);
            NodeSetPost(m_nodes[i], 0);
            NodeSetDistance(m_nodes[i], numeric_limits<double>::infinity());
        }
    }

    void scan(const string &file) {
        ifstream myFile;
        myFile.open(file);
        string c1, c2, c4;
        double c3;
        size_t id = 0;
        int numberOfEdges = 0;
        // using hash table
        unordered_map<string, size_t> nodeMap;

        if (myFile.is_open()) {
            // goes through file and adds nodes to a hash table
            while (myFile >> c1 >> c2 >> c4) {
                numberOfEdges++;
                // Find node to make sure it is not already found
                unordered_map<string, size_t>::const_iterator nodeA =
                    nodeMap.find(c1);

                // if nout found add to hash table
                if (nodeA == nodeMap.end()) {
                    pair<string, size_t> node1(c1, id++);
                    nodeMap.insert(node1);
                }

                unordered_map<string, size_t>::const_iterator nodeB =
                    nodeMap.find(c2);

                if (nodeB == nodeMap.end()) {
                    pair<string, size_t> node2(c2, id++);
                    nodeMap.insert(node2);
                }
            }

            // resizes Vectors to make inserting elements quicker and have
            // allcated space
            m_nodes.resize(nodeMap.size());
            m_adjList.resize(nodeMap.size());
            weightList.reserve(numberOfEdges);
            weightList.rehash(numberOfEdges);
            // weightArray.resize(nodeMap.size());
            // goes through hash table of nodes and adds them to m_nodes vector
            int i = 0;
            for (pair<string, size_t> node : nodeMap) {
                Node a(node.first, node.second);
                addNode(a);
                i++;
            }

            // reopen file
            myFile.close();
            myFile.open(file);

            // goes through file again and adds edges to the m_adjList vector
            while (myFile >> c1 >> c2 >> c4) {
                // find both nodes in hash table
                unordered_map<string, size_t>::const_iterator nodeA =
                    nodeMap.find(c1);
                unordered_map<string, size_t>::const_iterator nodeB =
                    nodeMap.find(c2);

                // get both nodes and add edge
                Node a = getNode(nodeA->second);
                Node b = getNode(nodeB->second);
                c3 = stod(c4);
                addEdge(a, b, c3);
            }
            myFile.close();
        }
    }

    void save(const string &file) const {
        // get file for outputing
        ofstream MyFile;
        MyFile.open(file);

        // goes through each nodes list and writes the edge tab-spaced
        for (size_t i = 0; i < m_nodes.size(); i++) {
            // get adjacent nodes
            Node a = getNode(i);
            list<Node> bList = getAdjNodes(a);

            // goes through list and writes it to output file
            for (list<Node>::iterator k = bList.begin(); k != bList.end(); k++) {
                unordered_map<string, double>::const_iterator weight = weightList.find(a.name() + "-" + k->name());
                // double weight = weightArray[a.id()][k->id()];
                MyFile << a.name() << "\t" << k->name() << "\t" << weight->second << endl;
            }
        }

        MyFile.close();
    }
};

std::ostream &operator<<(std::ostream &out, const Graph &g) {
    out << "Nodes in the graph:" << endl;
    for (unsigned i = 0; i < g.num_nodes(); i++) {
        Node a = g.getNode(i);
        out << a.name() << " Pre: " << a.getPre() << " Post: " << a.getPost() << " Distance: " << a.getDistance() << endl;
    }
    out << endl;
    out << "Adjacency list of the graph :" << endl;
    for (unsigned i = 0; i < g.num_nodes(); i++) {
        out << "Node " << g.getNode(i).name() << ", ID:" << g.getNode(i).id() << ": ";
        const list<Node> neighbors = g.getAdjNodes(g.getNode(i));
        for (list<Node>::const_iterator itr = neighbors.begin(); itr != neighbors.end(); ++itr) {
            Node a = g.getNode(i);
            Node b = *itr;
            double weight = g.getWeight(a, b);
            out << itr->name() << ": " << weight << ", ";
        }
        out << endl;
    }

    return out;
}