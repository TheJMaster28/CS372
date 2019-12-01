#include "Lab8.h"

double infin() { return numeric_limits<double>::infinity(); }

double distance(Node &a, Graph &G) { return G.NodeGetDistance(a); }

void distance(Node &a, Graph &G, double dis) { G.NodeSetDistance(a, dis); }

list<Node> make_Queue(Graph &G) {
    list<Node> Pqueue;
    for (size_t i = 0; i < G.num_nodes(); i++) {
        Pqueue.push_back(G.getNode(i));
    }
}

Node deleteMin(list<Node> H, Graph &G) {
    list<Node>::iterator minN;
    double min = infin();

    // list<Node>::iterator k = bList.begin(); k != bList.end(); k++
    for (list<Node>::iterator a = H.begin(); a != H.end(); a++) {
        if (min > distance(*a, G)) {
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
                }
    }
}

void Dijkstra_heap(Graph &G, Node &s) {}

bool testall() {
    Graph g("test.txt");
    cout << g;
}

int main() {
    testall();
    return 0;
}